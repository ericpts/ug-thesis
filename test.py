#!/usr/bin/env python3
import subprocess
import os
import colorama
from colorama import Fore, Style

from pathlib import Path

def print_color(color, msg: str, **kwargs):
    print(color, end='')
    print(msg, **kwargs)
    print(Style.RESET_ALL, end='')

def run(*args, **kwargs) -> str:
    p = subprocess.run(args, **kwargs, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if p.returncode == 0:
        return p.stdout.decode('utf-8')

    print_color(Fore.RED, 'Error running command {}'.format(args))

    print('Captured stdout:\n')
    print(p.stdout.decode('utf-8'))

    p.check_returncode()

def run_android():
    exe = Path(os.getcwd()) / 'build' / 'thesis'
    android = Path(os.getcwd()) / 'test'/ 'fixtures' / 'android'
    test_dir = Path(os.getcwd()) / 'test' / '_current'

    print_color(Fore.BLUE, 'testing on android...')
    run('rm', '-rf', test_dir)
    run('cp', '-r', android, test_dir)

    thesis_output = run(exe, *list(map(str, test_dir.glob('*.class'))), '--in-place')
    (test_dir / 'thesis.log').write_text(thesis_output)

def run_tap():
    exe = Path(os.getcwd()) / 'build' / 'thesis'
    tap = Path(os.getcwd()) / 'test'/ 'fixtures' / 'tap'
    test_dir = Path(os.getcwd()) / 'test' / '_current'

    def run_homework(hw: path):
        print_color(Fore.BLUE, 'testing on {}...'.format(hw))
        run('rm', '-rf', test_dir)
        run('cp', '-r', hw, test_dir)

        thesis_output = run(exe, *list(map(str, test_dir.glob('*.class'))), '--in-place')
        (test_dir / 'thesis.log').write_text(thesis_output)

        run('java', 'main', cwd=hw)

    for f in tap.glob('*'):
        for g in f.glob('*'):
            run_homework(g)

def run_fixtures():
    exe = Path(os.getcwd()) / 'build' / 'thesis'
    test_dir = Path(os.getcwd()) / 'test' / '_current'
    for f in Path('test/fixtures').glob('project*'):
        print_color(Fore.BLUE, 'Running on {}'.format(f))

        run('rm', '-rf', test_dir)
        run('cp', '-r', f, test_dir)

        thesis_output = run(exe, *list(map(str, test_dir.glob('*.class'))), '--in-place')
        (test_dir / 'thesis.log').write_text(thesis_output)

        run(test_dir / 'test.sh', cwd=test_dir)

def init():
    colorama.init()

    print('Building...')
    run('bash', 'make.sh')
    run('make', cwd='build')
    run('python3', 'make.py', cwd='test')

def main():
    init()
    # run_fixtures()
    # run_tap()
    run_android()

if __name__ == '__main__':
    main()

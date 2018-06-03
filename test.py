#!/usr/bin/env python3
import subprocess
import os
import colorama

from pathlib import Path

def main():
    colorama.init()


    subprocess.run(['bash', 'build.sh'], check=True)
    subprocess.run(['make'], check=True, cwd='build')
    subprocess.run(['make'], check=True, cwd='test')

    exe = Path(os.getcwd()) / 'build' / 'thesis'

    for f in Path('test/fixtures').glob('*'):
        print(colorama.Fore.GREEN + 'Running on {}'.format(f))
        print(colorama.Style.RESET_ALL)

        subprocess.run(['rm', '-rf', 'out'], check=True)
        subprocess.run(['mkdir', 'out'], check=True)
        subprocess.run([exe, *list(map(str, f.glob('*.class'))), '--out', 'out'], check=True)
        subprocess.run([f / 'test.sh'], check=True)

        subprocess.run(['java', 'Main'], check=True, cwd='out')


if __name__ == '__main__':
    main()

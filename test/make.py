#!/usr/bin/env python3
import subprocess
from pathlib import Path

def main():
    for f in Path('.').glob('**/*.java'):
        dest = f.with_suffix('.class')

        if dest.exists() and f.stat().st_mtime <= dest.stat().st_mtime:
            continue
        print('Compiling {}...'.format(f))
        subprocess.run(['javac', f.name], check=True, cwd=f.parent)

if __name__ == '__main__':
    main()

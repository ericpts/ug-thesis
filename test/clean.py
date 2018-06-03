#!/usr/bin/env python3
import subprocess
from pathlib import Path

def main():
    for f in Path('.').glob('**/*.class'):
        print('Removing {}...'.format(f))
        subprocess.run(['rm', '-f', f])

if __name__ == '__main__':
    main()

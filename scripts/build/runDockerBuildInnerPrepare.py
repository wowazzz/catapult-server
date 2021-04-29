import argparse
from pathlib import Path

from environment import EnvironmentManager
from process import ProcessManager

DATA_VOLUME = Path('/data')
USER_HOME = Path('/usr/catapult')


def main():
    parser = argparse.ArgumentParser(description='catapult project build generator')
    parser.add_argument('--disposition', help='type of image to create', choices=('dev', 'private', 'public'), required=True)
    parser.add_argument('--dry-run', help='outputs desired commands without runing them', action='store_true')
    args = parser.parse_args()

    print('preparing {} image'.format(args.disposition))

    process_manager = ProcessManager(args.dry_run)
    environment_manager = EnvironmentManager(args.dry_run)

    is_dev_build = 'dev' == args.disposition
    if is_dev_build:
        for name in ['seed', 'scripts', 'resources']:
            environment_manager.copy_tree_with_symlinks(DATA_VOLUME / name, USER_HOME / name)

    bin_folder_names = ['bin', 'deps', 'lib']
    if is_dev_build:
        bin_folder_names.append('tests')

    for name in bin_folder_names:
        environment_manager.copy_tree_with_symlinks(DATA_VOLUME / 'binaries' / name, USER_HOME / name)

    process_manager.dispatch_subprocess(['ls', '-laF', USER_HOME])

    for name in ['seed', 'scripts', 'resources', 'bin', 'deps', 'lib', 'tests']:
        process_manager.dispatch_subprocess(['ls', '-laF', USER_HOME / name])


if __name__ == '__main__':
    main()

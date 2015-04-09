#!/usr/bin/python
import click
from os import listdir, path, remove
import json
try:
	import urllib.request
except  ImportError:
	import urllib
from subprocess import call
from shutil import copy, rmtree

@click.command()
@click.argument('install', nargs=-1, required=True)

def install(install):
	gpmdir = path.expanduser('~') + '/.gpm'
	if install[0] == 'install' or install[0] == 'i':
		for a in range(1, len(install)):
			found = False
			v = open(gpmdir + '/installed/_installed.gpm', 'r')
			lines = v.readlines()
			for line in lines:
				if line == install[a] + '\n':
					found = True
			if found != True:
				y = listdir(gpmdir + '/pkg')
				try:
					z = install[a] + '.gpm'
					x = y.index(z)
					click.echo('\033[0;32mPackage "%s" found, installing...\033[0m' % install[a])
					m = open(gpmdir + '/pkg/' + z, 'r')
					f = json.loads(m.read())
					p = gpmdir + '/tmp/' + install[a]
					try:
						urllib.urlretrieve(f['url'], p) 
					except AttributeError:
						urllib.request.urlretrieve(f['url'], p) 
					g = f['commands']
					for y in range(len(g)):
						call(g[y].format(FILENAME=p, GPMDIR=gpmdir), shell=True)
					m.close()
					copy(p, gpmdir + '/installed/')
					v = open(gpmdir + '/installed/_installed.gpm', 'a')
					v.write(install[a] + "\n")
					v.close()
					e = gpmdir + '/tmp/'
					for b in listdir(e):
						remove(e + str(b))
				except ValueError:
					click.echo('\033[0;31mPackage "%s" not found, skipping...\033[0m' % install[a])
			else:
				click.echo('\033[0;31mPackage "%s" is already installed!\033[0m' % install[a])
	elif install[0] == 'remove' or install[0] == 'r' or install[0] == 'delete' or install[0] == 'd' or install[0] == 'destroy':
		for a in range(1, len(install)):
			try:
				remove(gpmdir + '/installed/' + install[a])
				v = open(gpmdir + '/installed/_installed.gpm', 'r')
				lines = v.readlines()
				v.close()
				v = open(gpmdir + '/installed/_installed.gpm', 'w')
				for line in lines:
					if line != install[a] + '\n':
						v.write(line)
				v.close()
				click.echo('\033[0;32mPackage "%s" found, removing...\033[0m' % install[a])
			except OSError:
				click.echo('\033[0;31mPackage "%s" not found, skipping...\033[0m' % install[a])
		click.echo('Done.')
	elif install[0] == 'check':
		for a in range(1, len(install)):
			found = False
			v = open(gpmdir + '/installed/_installed.gpm', 'r')
			lines = v.readlines()
			for line in lines:
				if line == install[a] + '\n':
					found = True
			if found:
				click.echo('\033[0;32mPackage "%s" is installed.\033[0m' % install[a])
			else:
				click.echo('\033[0;31mPackage "%s" is not installed.\033[0m' % install[a])
	elif install[0] == 'list' or install[0] == 'l' or install[0] == 'installed':
		v = open(gpmdir + '/installed/_installed.gpm', 'r')
		click.echo('\033[1;30mINSTALLED PACKAGES')
		click.echo('==================\033[0m')
		click.echo(v.read())
		v.close()
	elif install[0] == 'die':
		click.echo('\033[0;31mThis will remove GPM and all its installed packages from your system. Are you sure you want to continue?')
		kill_prompt = str(raw_input('Anything other than "yes" will cancel. >'))
		if kill_prompt.lower() == 'yes':
			rmtree(gpmdir)
			click.echo('GPM was successfully removed from your system.\033[0m')
		else:
			click.echo('GPM\'s removal was cancelled.\033[0m')
	elif install[0] == 'help':
		click.echo('\033[1;30mUSAGE:\033[0m')
		click.echo('\t\033[0;32mgpm install [package] [package2]\033[0m')
		click.echo('\t\033[0;31mgpm remove [package] [package2]\033[0m')
		click.echo('\t\033[0;34mgpm check [package] [package2]\033[0m to see if [package] is installed')
		click.echo('\t\033[0;34mgpm list\033[0m to view all installed packages')
		click.echo('\t\033[0;34mgpm die\033[0m to completely remove')
	else:
		click.echo('\033[0;31mUnknown parameter "%s"\033[0m' % install)

if __name__ == '__main__':
	install()
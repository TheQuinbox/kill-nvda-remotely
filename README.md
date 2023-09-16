# Kill NVDA Remotely.

This is a simple set of Windows programs that lets you restart NVDA on a remote machine (typically because you've lost your NVDA Remote connection).

## Usage.

1. Download the latest release.
2. Unzip it to a folder of your choice.
3. Edit client.bat and server.bat to point to your prefered IP address and port.
	* note that the IP address is of the computer that will be remoted into.
4. Open the port on your router (if you want to be able to do this from somewhere that's not where ever your computer is).
5. Run server.bat (I tend to do this on a new virtual desktop).
6. Whenever you need to restart your NVDA on that machine, run client.bat.

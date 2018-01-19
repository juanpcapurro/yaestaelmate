# Ya está el mate

## why
Social validation, I guess ¯\_(ツ)_/¯

## Ingredients
* ESP8266
* Wires
* Battery
* One button
* Mate 
* Notification sender: libnotify or growl. Notification magic by [node-notifier](https://www.npmjs.com/package/node-notifier)

## How to use
1. Power up your mate.
1. If this is the mate's first time in the network:
    1. Connect to the network it has created, `gaucho`.
    1. Go to its default IP in a browser `192.168.4.1` or use its captive portal.
    1. Choose the network's SSID and type its password
1. If you need to change the cebador's IP address from the default `192.168.0.17`, you can do so by entering the mate's IP in a browser and using the form it hosts. 
To find the mate's IP, you might need to connect to it via a serial connection, `picocom` is the serial monitor that I use. 
The mate's baud rate is `115200`. It'll notify what IP address it's assigned when it boots up.

## TO-DO
- [ x ] README.
- [ x ] Configurable cebador IP.
- [  ] mDNS server, or something else to avoid having to connect to the mate via its serial connection in order to know its IP.
- [  ] Globally installable NPM package for the notification server.
- [  ] Configurable message (in the same form as the cebador's IP).
- [  ] Make the notification server, and not the cebador, responsible for knowing who the next mate belongs to. Might require an extra button.




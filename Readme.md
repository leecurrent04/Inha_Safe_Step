[nRF24L01 Reference](https://nrf24.github.io/RF24/index.html)

#########################################################################################
futari.ino
                          #############################
                          # CopyRight to Min Sup Kim  #
                          #############################

[Purpose]
Forbid to ride kickboard over two guys

[Hardware]
We would use 8 pressure sensors, so we need to have at least 8 pins

[Software]
Step on over two specific foot-shaped stickers, think of over two guys ride kickboard
--> send signal to stop

[Fuctions]
pin_setup(): Set up analog pins
print_db(): Show information pressure sensor's resistance data on Serial Monitor
over_two(): Judge over two guys riding on kickboard
#########################################################################################

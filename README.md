# Smart-Shopping-Cart
A prototype of a shopping cart that could autonomously follow a shopper and assist in managing billing.
Check out how it worked [here](https://drive.google.com/file/d/1N2eWWQR136eBMysrpxHwYjHMJS-YMWWp/view?usp=sharing)

The tracking was achieved by determining by the transmission time of signals between a transmitter to be carried by the shopper and receivers (located on shopping cart). The transmission was coordinated and timed by a microcontroller on the cart and was initiated by radio signals.
Based on the delay of the signals and the time difference between signal detection by the receivers on the cart, the controller was able to calculate the distance thus speed and direction of the source relative to it.

The following illustration shows how it worked.
[Operating principle](https://media.giphy.com/media/Ri2ruXjodQL7EWQUyn/giphy.gif)

To read more about how it works and how you could put it together (possibly make improvements), checkout the documentation.

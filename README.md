# Smart-Shopping-Cart
A prototype of a shopping cart that could autonomously follow a shopper and assist in managing billing.
Check out how it worked [here](https://drive.google.com/file/d/1N2eWWQR136eBMysrpxHwYjHMJS-YMWWp/view?usp=sharing)

The tracking was achieved by determining by the transmission time of signals (from the source ie a tracker carried by the shopper) to the receivers (located on shopping cart). The transmission was coordinated and timed by a microcontroller on the cart and was initiated by radio signals.
Based on the delay of the signal and the time difference the receivers on the cart received the signal from the source, the cart was able to calculate the distance and direction of the source relative to it.

The following illustration shows how it worked.
[Operating principle](https://media.giphy.com/media/Ri2ruXjodQL7EWQUyn/giphy.gif)

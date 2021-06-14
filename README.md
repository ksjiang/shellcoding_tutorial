# shellcoding_tutorial
A series of increasingly difficult x64 shellcoding challenges

To start each challenge, put a `./flag` file along with the challenge binary into the same directory and serve the process on a TCP port. (It's more fun if you can get a friend to host the challenge and the secret flag!) Intended solutions will be posted to [my webpage] (https://nave1337.herokuapp.com/index.html). For extra fun, try to solve the challenges without first looking at the source - i.e., starting only with the binary.

The goal of each challenge is only to read the contents of `./flag`. In particular, obtaining a shell is not required, although it may be easier for some challenges.

A tutorial on how to write basic x64 shellcode can be found [here](https://docs.google.com/document/d/1N390U5NI7rWPxZ69k1mOh6OrpLJmFpCFtai4ZfrVL_4).


## Challenge Descriptions

### `shellcode1`: Run of the mill
The basic, boilerplate shellcoding challenge.

### `shellcode2`: Boycotting the shell
I don't think a shell is required to read a file!

### `shellcode3`: Mute
What good is a shellcode that can't talk back?

### `shellcode4`: Partition
They say that if you sever the head of a [Hydra](https://en.wikipedia.org/wiki/Hydra_(genus)), another one will grow in its place.

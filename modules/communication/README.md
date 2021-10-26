#Communication primitives

Another 'Just Clicked' idea (dekha jaye to pura worldline sim aur uske parts aise hi aaye the aur h 😁)
Dated: 25th Oct 21
Context: 'Universe in a nutshell sun rha tha, kal nptel ml exam tha, blockchain wala internship khatm hua tha kuchh din pehle, to zeromq bhi sochha isme, actually bas iske baare me sochna shuru kiya'

> **Each MessageChannel** must have a 'MessageType' typedef (or a using statement, aliasing the message type for that channel)

```
EA = Entity A
EB = Entity B
```

1. MessagePipe
EA <-> MessagePipe <-> EB
Pipe use krega send aur receive ke liye (async ho to behtar h, maybe by 'watching' the pipe for updates?)

2. MessageSocket
EA <-> MessageSocket <-> EB
Sockets use krega
Socket ports limited h, isliye agar bahut bahut jyada entities ho to MessagePipe should be used

3. Gossip
      EA1  ED2  EC5
        \   |    /
EA <-> Gossip <-> EB
      /   |   \
    EA9  ET2  EQ5

4. Broadcast
      EA1  ED2  EC5
        \   |    /
EA <-> Broadcast <-> EB
      /   |   \
    EA9  ET2  EQ5

Broadcast & Gossip are basically same, jyada sochha nhi hu abhi, baad me krne ke liye h

1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines when a command's output is fully received by looking for an EOF marker. Techniques to handle partial reads include implementing a loop to read until that marker is received, and attaching a header file that says how much data to expect.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

Due to TCP not preserving message boundaries, the beginning and end of a command by using delimeters or length prefixed messages. If this is not handled right, the client may think that partial messages are actually complete, which is obviously bad for data reasons. Additionally clients might hang or wait indefinitely.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols maintain the state of a session across several requests and responses. THis allows the server to remember previous interactions. Stateless protocols do not remember previous transactions, meaning all necessary information has to be included in each request. 

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is unreliable because it has a lot of disadvantages. It has lower latency and reduced overhead when compared to TCP. This is fine for moments where speed matters much more than data loss, such as watching videos and gaming. 

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The OS provides a socket interface to allow aplpications to use network communications. Sockets allow applications to send and receive data over a network using standard syscalls such as socket, bind, listen, accept, connect, send, and recv. This interface supports a variety of networking needs.
# UFCFVK-15-2 Internet of Things  - Reflection

**TODO** : Write your reflection here.

Two of the most important aspects within this system are data validation and server parallelisation.

Firstly, data validation is a point in which I could of improved upon in this project. Data validation is important in real world applications where misformed or corrupted messages could lead to system crashes and data corruption within the server. In my project the message handling makes a lot of assumptions that data will be correctly formatted and have the exact expected sizes and data. For instance this occured during the development of this project when parsing lat and lon fields from client messages. This could be improved with validation checks to reject invalid data before being processed and potentially impacting the entire system.
Server parallelisation is paramount in this project as it is required for multiple clients to interact simultaneously, to do this I have implemented multithreading which has been sufficient within the scope of my implementation however would require additional work to scale on a commercial level. To improve this to a real world industry level a more scalable model would be preferred such as asynchronous transmission to reduce overhead.
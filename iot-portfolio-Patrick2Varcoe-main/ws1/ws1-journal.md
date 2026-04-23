# UFCFVK-15-2 Internet of Things - Worksheet 1 Journal

TODO: Add at least three entries on **three different dates** while working on your worksheet and detailing your progress:

Each entry should include:

* What you worked on that day (e.g., understanding HAL, creating sensor, display , etc)
* What issues did you face and how did you solve them? (e.g., debugging, researching, asking tutors for help, etc) Add links to any resources you found useful and as much detail as possible. Explore different approaches.
* What did you learn from the issues/solutions?
* After the second entry, reflect on how your approach to problem-solving has evolved and what you would do differently next time.

After creating one entry, commit and push your changes to your repository. **You should commit your journal entries no later than a week written.**

## 2026-04-13

* Initial work on WS1 of project
Started off by setting up the directory for all the required files and configuring the makefile to generate the Ebike data.
Then using the prep work as a base reimplemented the temperature sensor as a gps sensor 
Then I implemented the functionality of the client to read from the sensor once again this was based off the prep work and so only required some tweaking for this instance.
Then I worked on parsing the results from this to be compliant with the different formatting the new data provided (positions instead of temperature)
Then I worked on the initial construction of the client messages and familiarised myself with the utility files getFormattedTime function ready to implement it later.
For the first day I mainly just focussed on getting down a foundation to work on, mainly repurposing the preperation work. 
The reasearch I did on this day was mainly limited to the preperation work as it explained a lot of the required library functions and was very relevant to the project as well as some basic cpp sytax research, as it is not my most familiar language. For the sytax I mainly used w3schools.com as I find it very user friendly and often has good examples, for anything I could not find on there I used geeksforgeeks.org, which is very similar.
## 2026-04-15

* Follow up work and Test case
On this day I started by working on the test case programs for the project, similarly to the main program I initially based this off the work done in the preperation for this project, with the tests being similar to the tests done in prep-ws1 with changes made to fit the exact case. The main thing I struggled with here was stil just being a bit unfamiliar with cpp syntax and formatting.
I then improved the output of the eBike Client implementing the proper functionality for the getFormattedTime as well as including the lock status of the bike into the output
I used similar resources to reasearch for this day as well, with the only addition being some documentation on makefiles to try and clean it up as I find the syntax for that very confusing.
I cant say my approach to solving the problems in this project changed siginifcantly however if I were to complete this again I think the best way of improving would be to practice the use of many of the techniques used here in other projects, gaining more experience, mainly due to my unfamiliarity with cpp, its syntax and its functions. This is due to the fact that the biggest resource I used was the preperation work which provides examples of implementation and functionality used within this project.
## N/A

* This project only took two seperate days of work, primarily as the preperation work is very relevant and easily adapted into this project.
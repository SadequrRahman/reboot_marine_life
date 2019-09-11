## "Reboot the earth"

#### dataset
For this demonstration we are making an arbitrary dataset which is filled with data of water quality index(0-100), which is measured from the sensor data. [0 being the better]. Another dataset comprises of the fisheries data which gives an idea about fish reproduction region at a particular place at a particular time. For this demonstration, we are taking 150 intermediate nodes between two ports. And in each node, there are 5 points. So, eventually there are 25 different ways to move from one node to another. Our dataset is a 365*750 integer matrix, where each row means specific date and the coloums are water quality index of a particular node.

#### Program

For the demonstration, we are planning a ship route from position 1 to position 150, which will take 30 days. And for each movement from 1 position to another, we are chosing optimum nodes depending on the water quality index and red region for fish production.

#### Visualization

We are using colormap to visualize the nodes data, dark means good quality and light means bad quality. Here we are simulating two different colormap, one of which is associated with the fish reproduction region and other is not. Our optimum path selection will be connected through all the dark nodes on different dates for different position.

#### Results

The "pos" array consists of the nodes which the ship will follow.

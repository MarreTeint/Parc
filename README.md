# Simulation of an amusement park
C project carried out at Polytech Paris Saclay

## Subject
In this project, we consider an amusement park with the following zones:

- An entrance through which visitors must pass to enter and exit the park. At
When entering the park, each visitor must pay the sum of 30€. We specify that
all exits are final.

- Aisles allowing visitors to get to the various attractions. Visitors can stay as long as
Visitors can stay as long as they wish in the park's alleys.

- The so-called "free" attractions where visitors can access freely, without queuing, and where they can stay for as long as they wish.
without queuing, and where they can stay as long as they wish. These attractions can be, for example
These attractions can be, for example, animations. In this exercise, we consider that this type of attraction has no limit to the number of visitors.
We consider in this exercise that this type of attraction does not have a limit to the number of its visitors.

- Controlled" attractions for which visitors have to queue. We
We specify that visitors leave the queue to enter the attraction in the same order as they entered it.
as they entered. Of course, an attraction of this type can only admit a limited number of visitors.
of visitors. In contrast to 'free' attractions, once they have entered a 'controlled' attraction, visitors are
Unlike "free" attractions, once visitors enter a "controlled" attraction, they are obliged to stay for the duration of the attraction. However,
However, as each visitor has a certain level of patience, if the time spent queuing is too long it will
However, each visitor has a certain level of patience, so if the time spent queuing is too long it will affect their level of satisfaction and the chances of them returning to the park.

We will therefore design a program that simulates a day of operation for such an amusement park.
for such an amusement park. This will allow us to see the revenue generated as well as the level of
the level of satisfaction of the visitors.

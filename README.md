# SDL2_game_framework

Ludum Dare 51 - Every 10 Seconds

Must be 2D. 

Something happenes every 10 seconds
    top down or isometric 


1Sandbox
2Real-time strategy (RTS)
3Shooters (TPS)
4Role-playing (RPG, ARPG, and More)
5Simulation and sports

*6 Survival*
    - hunger ticks every 10 seconds,
    - enemies spawn every 10 seconds,
    - whole world does things every 10 seconds

Goal: break the 10 second cycles... somehow?

Survival typically has a world that the player does not know
    every 10 seconds a big bad enemy comes toward the player. Player wants to avoid it and survive until they are strong enough to kill it?

Growing stronger:
    - levelling up by killing things?
    - special abilities
    - finding items that augment their stats / abilities
    - making new items from existing stuff you have (nice to have idea, not a need to have)
    
settings themes:
    *earth invasion / aliens*

Alien invasion setting:  
    - We are a human
    - Earth?? Where on earth? -- Wherever is easy to procedurally generate
    - Cityscape with wilderness nearby
    - True top down
    - Aliens could like deploy portals, and those portals allow an alien to pass through once every 10 seconds, you need to kill the aliens AND the portals
    - Have a 10 second timer on screen to indicate when the game state changes
    - main goal, get stronger and find crashed alien ship to close main portal
    - You could have aliens that divide/reproduce every 10 seconds (2^N grows too fast!!)
    - 

Gameplay mechanics:
    - Aliens get stronger over time to keep the player from getting complacent
    - items / survival material spawns every 10 seconds
        - items, things you equip
            - guns
            - swords
            - armor
            - anything you wear
        - survival material
            - food
            - crafting material
                -wood
                -rocks
                -metal
                etc...
    - lean into C, have lots of entities

Needed for minimum viable product:
    - aliens spawning from portal
        - alien types: 
            - ranged
            - meele
    - player collecting things in the world
    - combat system
        - meele combat
        - projectile system
    - large map
        - field + trees initially


graphics: 
    *- game view center screen
    *- health indicator
    - menu for item equiping
    *- 10 second timer of some sort
    - text window to show what item is selected (tooltip on mouse)
    - pickup based on distance

gameplay:
    - WASD/arrow keys movement
    - mouse directional aiming
    - e/shift buttons for selection
    - i for inventory
    - m for map (if we get that far)
    - click for attack/ shoot
    - fog of war (?)
    - player moves the screen

Asset list:
    - trees -
    - player -
    - aliens - 
    - teleporter -
    - meele attack animation -
    - shooting animation -

How to structure a game mechanic around "every 10 seconds"?

Synonyms for every 10 seconds
- two times every 20 seconds
- Six times a minute
- 360 times an hour
- 8640 times a day

- You could have the player have to do something every 10 seconds(repetitive)
    - Player queues up certian things to happen every 10 seconds
- You could have something happen TO the player every 10 seconds
    - Have the game get progressively harder at 10 second intervals
- You could have the game only last 10 seconds
- You could have the game state change every 10 seconds
    - ex enemies spawn every 10 seconds
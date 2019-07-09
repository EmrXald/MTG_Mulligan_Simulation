# MTG_Mulligan_Simulation
A work in progress project designed to simulate large number of starting hands and mulligans in Magic:The Gathering. The goal of the project is to give statistical ground to stand on when discussing different types of mulligan in EDH. Simulates the Vancouver, London and Partial Paris mulligan at this time.

## DISCLAIMER
This project is work in progress and a project I am pursuing whenever I find the time and inspiration to work on it. At this time cards and decks can be submitted without programming knowledge but the conditions of a keepable hand are hard coded and to alter them you would require fundamental knowledge of the C++ language. My hope is to change this in the future and make it possible for anyone to create conditions, but that will be tricky work so do not expect it in the near future unfortunately.
However if you are interested in simulating certain conditions and have no programming knowledge you can always e-mail me at emrxald@gmail.com and I'll see what I can do when I have the time :)

### Features to work towards:
1. Writing the results to a readable format for data export into programs such as excel.
2. Abstracting the conditions of a keepable hand to allow users without programming knowledge to create conditions.

## Background
This small project spawned from the introduction of the London Mulligan to EDH in Magic:The Gathering and the discussion that followed. From discussion I have partaken in and listened to it has been described as an overall better mulligan than the Vancouver Mulligan, often referring to that it just "feels" better and leads to less non-games when having to mulligan to 5 or even 4 cards. On the other side I have heard concern of combo decks looking to become even more prevalent than before in EDH. I have heard parallels drawn to the Partial Paris Mulligan that was formerly used in EDH and how the London Mulligan is not as bad but will still prove to be problematic. On both sides I have rarely seen these discussion be backed up by any other arguments than those involving speculation and what one feels will happen. After having discussed with my friends(casual group of power 5-8) and listened to prolific commander personalities on youtube and various podcasts I grew tired of the lack of substantial evidance in the discussion and started working on this simulator.

## The Application
The application is a console application where you submit a card repository and a deck(currently supports JSON file format) and simulate a number of mulligans to see how often you end up with a keepable hand. At this time the hard-coded condition attempts to build a flash hulk combo in its starting hand. the combo it is looking for is viscera seer, flash, protean hulk, swamp and island. This is a strict and unreasonable condition but simulating it enough times will show the reliability of each type of mulligan.

in the full repository an example repository and deck are included(card_repo.json and flash_hulk_deck.json).

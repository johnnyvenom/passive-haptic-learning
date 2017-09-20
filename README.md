Passive Haptic Learning
=======================

This repo contains files and resources for a replication of the "Passive
Haptic Learning" study documented in the following paper: 

Seim, C., Estes, T., & Starner, T. (2015). Towards Passive Haptic Learning of piano songs. IEEE World Haptics Conference, WHC 2015, 445–450. http://doi.org/10.1109/WHC.2015.7177752

Available online at:
http://ieeexplore.ieee.org/document/7177752/ 

Hardware and software created in November 2016 by John Sullivan. 

### Contents

<!-- MarkdownTOC -->

- [Project resources overview](#project-resources-overview)
    - [Hardware](#hardware)
    - [Software](#software)
    - [Notes](#notes)
- [Experiment Design Fall 2017](#experiment-design-fall-2017)
    - [ToDos](#todos)
    - [Proposed schedule](#proposed-schedule)
    - [Notes/Questions](#notesquestions)

<!-- /MarkdownTOC -->

## Project resources overview

Resources for the project include the following hardware and software: 

### Hardware

- Prototype gloves were built using the haptic boards developed at IDMIL for the ilinx and Body:Suit:Score projects
- The boards were reconfigured for xBee wireless communication.
- A standard MIDI keyboard is used, connected to the host computer via USB.

### Software

- The PHL application is built with Max by Cycling '74. 
    + Required Max packages:
        * bach library for advanced MIDI functionality
        * CNMAT objects for OSC messaging
        * There may be other externals - check the Max console on load for additional dependencies. 
    + The application generates data log files, to be used in analysis
- Analysis will be carried out in Matlab (to be programmed)

### Notes

- The full experiment was never run, and at last step there were some software bugs that need to get worked out. 
- The gloves are currently intact, though the xBee boards are no longer configured, having been acquisitioned for other projects. 
- The application was written for the first PHL experiment from (Seim, et al., 2015) - for single note melodies, but not for the second (two-handed chorded passages)

----

## Experiment Design Fall 2017

We (Aditya and Johnny) plan to replicate this experiment this fall. Here is a list of todos, proposed schedule, and general notes and questions for carrying it out. 

### ToDos

- Preparation:
    + Glove update (can use same boards)
    + Update/build out Max application for 2nd experiment, and for proper data logging. 
    + Write Matlab analysis scripts (DTW analysis, etc.)
    + Complete ethics application
    + Recruitment - how/when/how many participants?
- Carrying out experiment: 
    + Schedule block of time at CIRMMT 822 or similar
- Analysis
- Write up report/paper
- Publish

### Proposed schedule

| Item                  | Who?      | How long?         | When          |
| --------------------- | --------- | ----------------- | ------------- |
| Glove update          | Aditya    | 2 days            | End Sept      |
| Update Max app (ex.1) | Johnny    | 2 days            | End Sept      |
| Build Max app (ex. 2) | Johnny    | 2 days            | End Sept      |
| Matlab analysis tools | Aditya    | 1 week?           | Oct wk 1      |
| Ethics application    | A/both    | 2 days            | End Sept      |
| Recruitment           | both      | 1 week            | Oct wk 1      |
| Participants          | both      | 1 week            | Mid Oct       |
| Analysis              | both      | 2 weeks           | Oct end/Nov   |
| Write up              | both      | 2 weeks           | Nov           |
| Publish/submit        | both      |                   | end of yr     |      

### Notes/Questions

- Do we do a) exact replication of b) both experiments? 
- Or do we connect with haptic evaluation work (motors vs UltraHaptics, etc.)?
- How many participants? 
- $$$ to pay participants?

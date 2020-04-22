# UROSSemLogInfoPlugin

**For UE4 Version: 4.22.1** 

This Plugin intends to get certain PoseStamped Information out of the UE4 Editor, publishing them under a given topic, and allow certain things to happen to SemLog Objects

## Installation

**Required Plugins:**
- [UROSBridge](https://github.com/robcog-iai/UROSBridge)
- [UUtils](https://github.com/robcog-iai/UUtils)

Download this Plugin and simply put it into your Projects Plugins folder.

## Usage

After importing the Plugin into the UE4 Editor, you now have a new Actor which you can place into your Level. It will begin it's work and advertise the topics and services beneath when you have dragged the AActorSemLogPublisher into the level.

It will publish a PoseStamped Message to: '/sem_log_plugin/posestamped_messages' (default, can be changed on the Actor itself).

It will advertise a service to delete all SemLog Objects in the Level under: '/sem_log_plugin/delete_all_objects' (default, can be changed on the Actor itself). 

## Current & Planned Features

- [x] PoseStamped Publisher, Info of every SemLog Tagged Object in which the Actor is in.
- [x] Remove all the SemLog Objects at once per Ros Service Call
- [] Check if the SemLog Object is stable.
    + [] Maybe a custom Message Type
- [] Check if the SemLog Object had an collision.
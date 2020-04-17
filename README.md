# USemLogObjectWorker

**For UE4 Version:__4.22.1__** 

This Plugin intends to get certain PoseStamped Information out of the UE4 Editor, publishing them under a given topic, and allow certain things to happen to SemLog Objects

## Installation

**Required Plugins:**
- [URosBridge](https://github.com/robcog-iai/UROSBridge)
- [UUtils](https://github.com/robcog-iai/UUtils)

Download this Plugin and simply put it into your Projects Plugins folder.

## Usage

### PoseStamped Publisher

After importing the Plugin into the UE4 Editor, you now have a new Actor which you can place into your Level. It will publish a PoseStamped Message to default: '/sem_log_pose'.

1. Drag the new 'AActorSemLogPublisher' into your level
2. (optional) Click on your placed Actor in the World Outliner and change the Topic to publish it to. 

## Current Features

- Publish PoseStamped of every SemLog Object in which the Actor is in.

## Planned Features

- Remove all the SemLog Objects at once per Ros Service Call
- Determine if a SemLog Object is stable.
# iPhone-RootFS-tool

## What does this tool do? 
- iPhone-rootFS-Tool will transfer all of the directories and files of your iOS device root directory to your computer.
- This can happen before first unlock (BFU) or after unlocking (AFU) your iOS device. 

## Requirements
- An iOS device that can be jailbroken with [checkra1n](https://checkra.in). 
- USB 
- Storage
    - The archive can be quite large (GB's in size). Typically AFU dumps will contain more data as well. 
- Time
    - This process isnt quick and we are open to suggestions on how to improve!

## Installation & Usage
- Clone this repository
- Usage:
    - Generate archive in current working directory: `./iPhone-rootFS-tool`
    - Generate archive with specified output directory: `./iPhone-rootFS-tool -o <Your Output Directory>`

## Build from source
- Run: `gcc main.c iLibX.c -o iPhone-rootFS-tool`

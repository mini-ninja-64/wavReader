<h1>Currently Implemented</h1>

* Wave file type (struct)
* Opens waves and gets data like: num of channels, sample rate etc
* Copies audio to char array
* Calculate the amplitudes of samples
* Calculate Track Length (Basic)
* Generate spectogram (basically a test for me)
* Accepts formats with an LIST(INFO) chunk (possibly reimplement whole reader to read file by chunk to gather info that way file could be in any order and skip unkown bytes)
* Split up samples by channel instead of raw audio data (could use a better solution)
* add ncurses stuff (very basic)

<h1>To Do</h1>

* Update C version
* Stream from disk
* Support non pcm formats
* Create Branches for stuff
* Come up with a solution for playback
* Lots more

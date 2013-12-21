# Bogo in C

This is pretty much a "translation" from the [Python engine][1]. We plan to make
this version the common code base for all supported platforms.

We have to move away from Python since there is no viable method of running
Python on [Firefox OS][2] (effectively a browser). We choose C because it runs
on all desktop operating systems and can be compiled to Javascript/ams.js through
[Emscripten][3]. In case one of us feel like supporting iOS or Android then it
would still be straightforward.

[1]: https://github.com/BoGoEngine/ibus-bogo-python/
[2]: http://www.mozilla.org/en-US/firefox/os/
[3]: https://github.com/kripken/emscripten

## Contributing

See [HACKING.md](HACKING.md)

## License

Trung Ngo <ndtrung4419@gmail.com> is currently the main/sole developer and he
takes pleasure in releasing this project under GPL v3 **all the way down!**


[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/lewtds/bogo-c/trend.png)](https://bitdeli.com/free "Bitdeli Badge")


# One Billion Row Challenge

- https://github.com/gunnarmorling/1brc/blob/db064194be375edc02d6dbcd21268ad40f7e2869/README.md

This is my attempt to through crunch lots of data, using C.

## How to run?

1. Download [world cities](https://simplemaps.com/data/world-cities) (Basic)
2. Unzip `worldcities.csv`
3. Keep only the `city_ascii` column, remove headers, save as ANSI encoding

4. ```sh
    docker build -t 1brc . && docker run -it --init --rm -v "%cd%\input.txt":/usr/src/app/data/input.txt:ro 1brc ./app ./data/input.txt
    ```

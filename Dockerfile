FROM gcc:13.4.0
COPY . /usr/src/app
WORKDIR /usr/src/app
RUN make
CMD ["./build/app"]

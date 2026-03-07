FROM gcc:13.4.0 AS builder

COPY . /usr/src/app
WORKDIR /usr/src/app
RUN make

FROM scratch
WORKDIR /usr/src/app
COPY --from=builder /usr/src/app/worldcities.csv ./worldcities.csv
COPY --from=builder /usr/src/app/build/app ./app
CMD ["./app"]

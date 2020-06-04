FROM raspbian/stretch:latest

RUN apt-get update && \
apt-get install -y \
cmake \
build-essential \
wiringpi \
wget \
unzip \
git

RUN wget https://github.com/joan2937/pigpio/archive/master.zip && \
  unzip master.zip && \
  cd pigpio-master && \
  make && \
  make install




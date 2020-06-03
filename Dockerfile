FROM raspbian/stretch:latest

RUN apt-get update && \
apt-get install -y \
cmake \
build-essential \
wiringpi
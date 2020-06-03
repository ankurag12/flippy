winpty docker run -it --rm\
  --network host \
  -v "/$(pwd -W):/home/flippy" \
  --workdir="//home/flippy" \
  rpi-flippy:latest \
  bash

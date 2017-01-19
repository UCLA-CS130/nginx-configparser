FROM ubuntu:14.04

MAINTAINER Richard Min <richardmin97@gmail.com>

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install vim g++ git -y

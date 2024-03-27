FROM hyperwin/hyperosbuildenv:1.0

ENV DEBIAN_FRONTEND noninteractive

WORKDIR /hyperos

COPY scripts/Makefile .

RUN make
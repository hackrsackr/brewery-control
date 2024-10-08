FROM python:3.9 AS base

COPY ./dist /app/dist
COPY ./requirements.txt /app/requirements.txt

ENV PIP_EXTRA_INDEX_URL=https://www.piwheels.org/simple
ENV PIP_FIND_LINKS=/wheeley

RUN set -ex \
    && mkdir /wheeley \
    && pip3 install --upgrade pip wheel setuptools \
    && pip3 wheel --wheel-dir=/wheeley -r /app/requirements.txt \
    && pip3 wheel --wheel-dir=/wheeley /app/dist/*

FROM python:3.9-slim
EXPOSE 5000
WORKDIR /app

ARG service_info=UNKNOWN
ENV SERVICE_INFO=${service_info}

COPY --from=base /wheeley /wheeley

RUN set -ex \
    && pip3 install --no-index --find-links=/wheeley brewblox-meters \
    && rm -rf /wheeley \
    && pip3 freeze

ENTRYPOINT ["python3", "-m", "brewblox_meters"]

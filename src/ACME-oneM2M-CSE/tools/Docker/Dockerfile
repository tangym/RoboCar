FROM python:3.8

RUN apt-get update && apt-get -y update

RUN mkdir acme-cse
COPY tools/Docker/acme.docker acme-cse/acme.ini
COPY acme/ acme-cse/acme/
# COPY apps/ acme-cse/apps/
COPY init/ acme-cse/init/
COPY requirements.txt requirements.txt

RUN python3 -m pip install -r requirements.txt
WORKDIR acme-cse/

CMD ["python3", "-m", "acme"]
EXPOSE 8080

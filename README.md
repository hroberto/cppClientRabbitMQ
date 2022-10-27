# cppClientRabbitMQ
Exemplo de integração com o Broker RabbitMQ.

## 1. Referencias

[Installing Environment Developer](https://github.com/hroberto/wiki/blob/main/devops/DEV%20-%20Installing%20Environmet%20Developer.md)
[Docker - Install](https://github.com/hroberto/wiki/blob/main/devops/Docker%20-%20Install.md)
[RabbitMQ - Install e Samples](https://github.com/hroberto/wiki/blob/main/brokes/Broker%20-%20RabbitMQ-%20Install.md)
[RabbitMQ - Samples Coding](https://rabbitmq-c.docsforge.com/master/amqp_rpc_sendstring_client.c/)
-------------

## 2. Construindo o projeto

- Instruções:
```bash
git clone https://github.com/hroberto/cppClientRabbitMQ.git
cd cppClientRabbitMQ

rm -fr {buildDebug,buildRelease}
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B buildDebug
# cmake -DCMAKE_BUILD_TYPE=Release  -S . -B buildRelease

cd buildDebug
make
```

-------------

## 3. Subindo ambiente

> Pre-requisito:  [Docker - Install](https://github.com/hroberto/wiki/blob/main/devops/Docker%20-%20Install.md)

- Start containers
```
cd Docker
mkdir -p ../bin/grafana/{grafana_data,provisioning}
sudo chmod 777 -R ../bin/grafana

# start 
sudo docker-compose up  -d

## view logs
docker-compose logs grafanaservices


```

- Configurando o Grafana com Prometheus

```ini
####
[Prometheus]
- Visualizar o status targets:  http://localhost:9090/targets?search=

[Grafana]
- realizar o login no grafana:
    http://localhost:3000/datasources/new?utm_source=grafana_gettingstarted
    # login: admin
    # password: admin

- Criar o novo DataSource tipo "Prometheus"
    - Entrar com [HTTP URL] = http://prometheus:9090
    - "Save & test"

- Importar o modelo DashBoard para RabbitMQ ( Menu "Dashboards / Import" )
    - Enter: [ ID ] = 10991
    ## Notes: find in https://grafana.com/grafana/dashboards/?search=rabbitmq
    - Select the Data Source Prometheus

```

- Stop containers
```bash
# stop
docker-compose down
```

## 4. Compilando e executando

```bash

./bin/Debug/sampleRPC/rpc-server &

# duas instancias client
./bin/Debug/sampleRPC/rpc-client &
./bin/Debug/sampleRPC/rpc-client &

```


## 5. Gerenciando do RabbitMQ via Console

- [Guia rabbitmqctl](https://www.rabbitmq.com/rabbitmqctl.8.html)



```bash
docker ps 
# CONTAINER ID   IMAGE                        COMMAND                  CREATED       STATUS       PORTS                                                                                                                                                 NAMES
# f6cdbc796c1b   grafana/grafana              "/run.sh"                4 hours ago   Up 4 hours   0.0.0.0:3000->3000/tcp, :::3000->3000/tcp                                                                                                             grafana
# 068f6102fbbd   bitnami/prometheus           "/opt/bitnami/promet…"   4 hours ago   Up 4 hours   0.0.0.0:9090->9090/tcp, :::9090->9090/tcp                                                                                                             prometheus
# 324f0e0e6421   rabbitmq:3.8.16-management   "docker-entrypoint.s…"   4 hours ago   Up 4 hours   4369/tcp, 5671/tcp, 0.0.0.0:5672->5672/tcp, :::5672->5672/tcp, 15671/tcp, 15691-15692/tcp, 25672/tcp, 0.0.0.0:15672->15672/tcp, :::15672->15672/tcp   rabbitmq

docker exec -ti rabbitmq /bin/bash

```

- Fechando uma conexao

```bash
rabbitmqctl  list_connections pid user peer_host state
# Listing connections ...
# pid     user    peer_host       state
# <rabbit@rabbitmq.1666699416.10775.0>    guest   xxx.xx.xx.xx      closed
# <rabbit@rabbitmq.1666699416.10791.0>    guest   xxx.xx.xx.xx      running

rabbitmqctl  close_connection   '<rabbit@rabbitmq.1666699416.10791.0>'    'automatic close'
# Closing connection <rabbit@rabbitmq.1666699416.10791.0>, reason: automatic close...
```


- Listando exchanges atuais

```bash

rabbitmqctl   list_exchanges  name type durable auto_delete internal arguments policy
# Listing exchanges for vhost / ...
# name            type    durable auto_delete     internal        arguments       policy
# amq.topic       topic   true    false   false   []
# amq.fanout      fanout  true    false   false   []
# amq.direct      direct  true    false   false   []
#                 direct  true    false   false   []
# amq.match       headers true    false   false   []
# amq.rabbitmq.trace topic   true    false   true    []
# amq.headers     headers true    false   false   []


for IT in `rabbitmqctl  list_exchanges name | grep -v 'Listing\|name\|amq'` ; do echo ${IT};  rabbitmqadmin  delete exchange  name="${IT}"; done


```

- Apagando uma fila

```bash


rabbitmqctl  delete_queue   test.client2 ; \
rabbitmqctl  delete_queue  test.client1
rabbitmqctl  list_channels connection messages_unacknowledged


for IT in `rabbitmqctl list_queues  name | grep -v 'Timeout\|Listing\|name'`; do echo ${IT}; rabbitmqadmin delete queue name="${IT}"; done

```

- Listando configurações gerais

```bash

rabbitmqctl  report


```



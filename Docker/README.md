

## Start Docker Compose

```bash
cd Docker
mkdir -p ../bin/grafana/{plugins,provisioning}
sudo chmod 777 -R ../bin/grafana


# start 
docker-compose up  -d

# stop
docker-compose down


```

## Configure Application

[Config Prometheus](https://www.rabbitmq.com/prometheus.html#overview-prometheus)

- Prometheus
    http://localhost:9000

- Grafana
    http://prometheus:3000
    http://localhost:3000
    # login: admin
    # password: admin

    DATA SOURCES: Add your first data sourcer / Prometheus
    # URL: http://prometheus:9090

    # Copy ID Dash Dashboards "RabbitMQ-Overview"
    https://grafana.com/grafana/dashboards/?search=rabbitmq
    ## ID: 10991
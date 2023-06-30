# ITCH-Nasdaq Cloud Data Services Kafka

## Original Nasdaq Cloud Data Services SDK Python
```https://github.com/Nasdaq/NasdaqCloudDataService-SDK-Python```

## Client Authentication
Fill out credentials in ```client-authentication-config.json```

```
{
    "oauth.token.endpoint.uri": "",
    "oauth.client.id": "",
    "oauth.client.secret": ""
}
```

## How to use
```
make build_container
make run_container
docker exec -it ${docker_container_name} bash   // check by "docker container ls"
make setup
make ubuntu_unit_tests
```

### Python parser
```
cd python_parser
python3 parse_trade_message.py  // check the data dates
```
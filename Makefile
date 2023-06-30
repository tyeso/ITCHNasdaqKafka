ubuntu_unit_tests:
	cd template; python3 ./generate.py -a 1; cd ..
	rm -rf build
	mkdir build
	cp conan/clang_ubuntu ~/.conan/profiles/clang
	cd build && conan install .. --profile clang --update && cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-DENABLE_TESTING=On .. && \
		make unit_tests -j 8 && \
		./bin/unit_tests -s -r compact

prune_container:
	docker container prune -f

build_container:
	docker build -t nasdaqkafka .

run_container:
	docker run \
		--mount type=bind,source="/tmp",target="/tmp" \
		--mount type=bind,source="${PWD}",target="/home/nasdaqkafka" \
		--network="host" \
		--ulimit memlock=100000000000:100000000000 \
		-dit nasdaqkafka

setup:
	wget https://github.com/zeromq/libzmq/archive/refs/heads/master.zip -O /home/libzmq-master.zip && \
	unzip /home/libzmq-master.zip -d /home/ && \
	cd /home/libzmq-master && \
	rm -rf build && \
	mkdir build && \
	cd build && \
	cmake .. && \
	make -j 4 install

	cd /home/nasdaqkafka && \
	pip install -e. --user

	cd template && \
	python3 mako_itch_specs_maker.py; 
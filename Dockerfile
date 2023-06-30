FROM nvidia/cuda:11.6.0-cudnn8-devel-ubuntu20.04

# timezone data
ENV TZ=Asia/Seoul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y apt-transport-https ca-certificates gpg wget curl unzip tar g++ make git vim && \
    apt-get install -y clang-12 clang-format-12 clang-tidy-12 clang-tools-12 && \
    apt-get install -y doxygen && \
    apt-get install -y lcov gcovr && \
    apt-get install -y zstd && \
    apt-get install -y libasan6

RUN ln -s /usr/bin/clang-tidy-12 /usr/bin/clang-tidy
RUN ln -s /usr/bin/clang-format-12 /usr/bin/clang-format
# install libzmq
RUN apt-get install -y libzmq3-dev

#  download libtorch  #
WORKDIR /opt
ENV LIBTORCH_URL=https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.12.1%2Bcpu.zip
RUN curl -fsSL --insecure -o libtorch.zip  $LIBTORCH_URL \
    && unzip -q libtorch.zip \
    && rm libtorch.zip
ENV TORCH_HOME=/opt/libtorch

#  install tmux, vimrc
RUN apt install -y tmux htop
RUN cd \
    && git clone https://github.com/gpakosz/.tmux.git \
    && ln -s -f .tmux/.tmux.conf \
    && cp .tmux/.tmux.conf.local .
RUN git clone --depth=1 https://github.com/amix/vimrc.git ~/.vim_runtime \
    && sh ~/.vim_runtime/install_awesome_vimrc.sh



# install python=3.9
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository -y ppa:deadsnakes/ppa && \
    apt-get update && \
    apt install -y python3.9 && \
    apt-get install -y python3.9 python3-pip && \
    ln -s -f /usr/bin/python3.9 /usr/bin/python3 && \
    ln -s -f /usr/bin/python3.9 /usr/bin/python && \
    ln -s -f /usr/bin/pip3 /usr/bin/pip

# Install python3 libraries #
RUN pip install install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cpu
RUN pip install pyyaml matplotlib scipy numpy
RUN pip install conan gym jinja2 Pygments mako

RUN pip install zmq

RUN conan user
RUN mkdir -p ~/.conan/profiles

#  download and install cmake 3.24 #
# Installing cmake 3.24 because if we install with apt-get we get the old cmake 3.16..
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null

RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null

RUN apt-get update && \
    rm /usr/share/keyrings/kitware-archive-keyring.gpg && \
    apt-get install -y --force-yes kitware-archive-keyring && \
    apt-get install -y cmake


# cpp checker
RUN apt-get install -y ccache cppcheck
RUN apt-get install -y graphviz

# zstandard (I wasn't able to install it with CMake or Conan...)
RUN apt-get install -y libzstd-dev

# for libzmq, need pkg-config
RUN apt-get install -y pkg-config

# IB TWS 954 later can listen on port 7496 (live API) and 7497 (paper API)
# this functionality is disabled by default in TWS configuration
EXPOSE 7496
EXPOSE 7497

#  cuda env  #
# export allows automatic cuda arch detection by cmake
ENV PATH /usr/local/cuda-11.6/bin:$PATH
ENV LD_LIBRARY_PATH /usr/local/cuda-11.6/lib64:$LD_LIBRARY_PATH

WORKDIR /home/nasdaqkafka

CMD ["bash"]

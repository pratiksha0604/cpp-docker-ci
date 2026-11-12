# ---- Stage 1: build ----
FROM gcc:13 AS build
RUN apt-get update && apt-get install -y --no-install-recommends cmake && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . .
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build -j"$(nproc)" \
 && ctest --test-dir build --output-on-failure

# ---- Stage 2: minimal runtime ----
FROM debian:bookworm-slim
RUN useradd -m appuser
WORKDIR /app
COPY --from=build /app/build/server /app/server
USER appuser
EXPOSE 8080
CMD ["/app/server"]

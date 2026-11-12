# C++ Docker CI/CD Pipeline

**Author:** Pratiksha Bhende (GitHub: [pratiksha0604](https://github.com/pratiksha0604))

A small C++17 web server (routes `/`, `/about`, `/projects`, `/health`) with unit tests, a multi-stage Dockerfile, and a GitHub Actions workflow that tests the code, builds a Docker image tagged with the commit SHA, pushes it to Docker Hub, and optionally redeploys on Railway.

## Project structure
```
cpp-docker-ci/
├── .github/workflows/ci.yml
├── src/            (main.cpp, router.cpp, router.hpp)
├── tests/          (test_router.cpp)
├── CMakeLists.txt
├── Dockerfile
└── README.md
```

## Run locally
```bash
cmake -S . -B build && cmake --build build
ctest --test-dir build --output-on-failure
./build/server            # http://localhost:8080
```

With Docker:
```bash
docker build -t cpp-docker-ci .
docker run -p 8080:8080 cpp-docker-ci
```

## CI workflow (`.github/workflows/ci.yml`)

| Job | Runs when | What it does |
|-----|-----------|--------------|
| `test` | every push / PR to `main` | Builds the C++ project inside a `gcc:13` container with CMake and runs `ctest` |
| `docker` | push to `main`, only if `test` passes | Logs in to Docker Hub, builds the image, pushes it tagged with the commit SHA (`:<sha>`) and `:latest` |
| `deploy` | after `docker` succeeds (optional) | Calls Railway's GraphQL API to redeploy the service; skipped if `RAILWAY_TOKEN` is missing |

### Required GitHub secrets
(Repo → Settings → Secrets and variables → Actions)

- `DOCKERHUB_USERNAME` – your Docker Hub username
- `DOCKERHUB_TOKEN` – a Docker Hub access token (Account Settings → Security)
- *(optional)* `RAILWAY_TOKEN`, `RAILWAY_SERVICE_ID`, `RAILWAY_ENVIRONMENT_ID` – for the Railway deploy step

### How it flows
1. Push code to `main`.
2. `test` compiles and runs unit tests in a container.
3. If tests pass, `docker` builds and pushes `username/cpp-docker-ci:<commit-sha>`.
4. `deploy` (optional) tells Railway to pull the new image and redeploy.

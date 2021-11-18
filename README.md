# CppParser
A c++ dependency injection/parsing library

## Running Tests Locally
The tests can be run locally using an IDE or cmake directly (ctest command).  To run the tests using the testing environment (docker), first make sure that [Docker](https://www.docker.com) installed.

```bash
# Build the docker testing image
docker build -t testing_image -f DockerTestFile .

# Run the built tests and view results
docker run --rm testing_image 

```
# CppParser
A c++ dependency injection/parsing library.  This library allows you to register that your class that meets an interface requirement.  When registering your class you must describe how to build your class (the arguments, their names, and a brief description).  

For instance, in the following example the ParsedFunction implements MathFunction where it takes a single string argument for the formula. Register options are described in the following sections.

```c++
#include "parser/registrar.hpp
REGISTER_DEFAULT(ablate::mathFunctions::MathFunction, ablate::mathFunctions::ParsedFunction, "a string based function to be parsed with muparser",
ARG(std::string, "formula", "the formula that may accept x, y, z, t"));
```
A parser or factory be used to create instances of the required interface with specified arguments recursively based upon a parser.  A yamlParser as been implemented.  The following snippet shows creating the parser and getting an instance from the yaml root.  For the yaml parser classes are designated with tags.

```c++
std::shared_ptr<cppParser::YamlParser> parser = std::make_shared<cppParser::YamlParser>(inputFilePath);
auto enumerator = parser->GetByName<enumerator::Enumerator>("");
```

```yaml
---
name: exampleFileGen.
enumerations:
  - !enumerator::enumerations::List
    name: mu
    values: [ .1, .2, .3 ]
  - !enumerator::enumerations::List
    name: density
    values: [ 1.1, 1.2 ]
generator:
  !enumerator::generators::FileTemplateGenerator
  path: template.sh
reporters:
  - !enumerator::reporters::StreamReporter

```

A series of Macros have been defined to help simplify registering classes.  Information is available in the [registar.hpp](src/registrar.hpp).

## Running Tests Locally
The tests can be run locally using an IDE or cmake directly (ctest command).  To run the tests using the testing environment (docker), first make sure that [Docker](https://www.docker.com) installed.

```bash
# Build the docker testing image
docker build -t testing_image -f DockerTestFile .

# Run the built tests and view results
docker run --rm testing_image 

```
#include <CLI/CLI.hpp>

import init;
import build;

int main(int argc, char **argv) {
  CLI::App app{"C++ build system and package manager"};

  // init command section -- start
  const auto initCmd =
      app.add_subcommand("init", "Initialize a new project")->alias("i");

  std::string projectName;
  initCmd->add_option("project_name", projectName, "Name of the project");
  initCmd->callback([&]() { init::process(projectName); });
  // init command section -- end

  // build command section -- start
  app.add_subcommand("build", "Compile the current project")
      ->alias("b")
      ->callback([&]() { build::process(); });
  // build command section -- end

  CLI11_PARSE(app, argc, argv);
  if (argc == 1)
    std::cout << app.help() << std::endl;

  return 0;
}

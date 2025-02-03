{
  description = "Task cli development flake";

inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }: 
  let 
    system = "x86_64-linux"; 
    pkgs = import nixpkgs { inherit system; };
  in {
    devShells.${system}.default = pkgs.mkShell {
      buildInputs = [
        pkgs.cmake
        pkgs.makeWrapper
        pkgs.gcc
        pkgs.clang
        pkgs.fish
      ];

      shellHook = ''
        echo "Welcome to the C/C++ development environment!"
        exec fish
      '';
    };
  };
}

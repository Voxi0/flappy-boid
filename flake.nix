{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    systems.url = "github:nix-systems/default";
  };
  outputs = inputs: let
    forEachSystem = inputs.nixpkgs.lib.genAttrs (import inputs.systems);
    pkgs = forEachSystem (system: import inputs.nixpkgs {inherit system;});
  in {
    devShells = forEachSystem (system: {
      default = pkgs.${system}.mkShellNoCC {
        nativeBuildInputs = with pkgs.${system}; [
					clang
          compiledb
          pkg-config
          raylib

          emscripten
          python3
          nodejs
        ];
      };
    });
  };
}
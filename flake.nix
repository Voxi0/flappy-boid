{
	inputs = {
		nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
		systems.url = "github:nix-systems/default";
		flake-parts.url = "github:hercules-ci/flake-parts";
	};
	outputs = inputs: inputs.flake-parts.lib.mkFlake {inherit inputs;} {
		systems = import inputs.systems;
		perSystem = {pkgs, ...}: {
			devShells.default = pkgs.mkShellNoCC {
				nativeBuildInputs = with pkgs; [
					compiledb
					gcc
					pkg-config
					raylib
				];
			};
		};
	};
}
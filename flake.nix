{
  description = "emexos dev flake";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    systems.url = "github:nix-systems/default";
  };
  outputs = inputs: let
    forEachSystem = inputs.nixpkgs.lib.genAttrs (import inputs.systems);
    pkgs = forEachSystem (system: import inputs.nixpkgs {inherit system;});
  in {
    devShells = forEachSystem (
      system: {
        default = pkgs.${system}.mkShellNoCC {
          nativeBuildInputs = with pkgs.${system}; [
            pkgsCross.x86_64-embedded.stdenv.cc
            nasm
            libisoburn
            qemu
            git
            wget
          ];
        };
      }
    );
  };
}

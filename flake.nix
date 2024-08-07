{
  description = "Nix flake for IPA2X project";
  inputs.nixpkgs.url = "github:lopsided98/nixpkgs/nix-ros";
  inputs.nix-ros = {
      url = "github:lopsided98/nix-ros-overlay";
      inputs.nixpkgs.follows = "nixpkgs";
  };
  inputs.flake-utils.url = "github:numtide/flake-utils";
  outputs = { self, nixpkgs, nix-ros, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ nix-ros.overlays.default ];
        };
        fastdds = pkgs.callPackage ./nix/fastdds.nix {};
      in {
        devShells.ros =
          pkgs.mkShell {
            nativeBuildInputs = [ pkgs.fastddsgen ];
            buildInputs = with pkgs.rosPackages.noetic; [
              ros-core
            ];
          };
        packages = rec {
          default = ipa2x-warning;
          inherit fastdds;
          ipa2x-warning = pkgs.stdenv.mkDerivation rec {
            name = "ipa2x-warning";
            src = ./warning_cpp;
            nativeBuildInputs = [ pkgs.cmake ];
            buildInputs = [ fastdds pkgs.argparse ];
          };
        };
      }
    );
}

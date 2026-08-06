{
  description = "Nix Helper - TUI utility for Nix/NixOS package management";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        packages = {
          nixhelp = pkgs.stdenv.mkDerivation {
            pname = "nixhelp";
            version = "1.0.0";
            src = ./.;
            
            nativeBuildInputs = [ pkgs.gcc pkgs.ncurses ];
            buildInputs = [ pkgs.ncurses ];
            
            buildPhase = ''
              g++ -std=c++17 -O2 -o nixhelp src/main.cpp -lncurses
            '';
            
            installPhase = ''
              mkdir -p $out/bin
              cp nixhelp $out/bin/
            '';
            
            meta = with pkgs.lib; {
              description = "TUI utility for Nix/NixOS package management";
              homepage = "https://github.com/example/nixhelp";
              license = licenses.mit;
              maintainers = [ maintainers.example ];
              mainProgram = "nixhelp";
            };
          };
          
          default = self.packages.${system}.nixhelp;
        };
        
        apps.default = flake-utils.lib.mkApp {
          drv = self.packages.${system}.nixhelp;
        };
        
        devShells.default = pkgs.mkShell {
          buildInputs = [ pkgs.gcc pkgs.ncurses pkgs.gdb ];
          shellHook = ''
            echo "Development shell for nixhelp"
            echo "Build with: g++ -std=c++17 -O2 -o nixhelp src/main.cpp -lncurses"
          '';
        };
      }
    );
}

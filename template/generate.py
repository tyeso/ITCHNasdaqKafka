from mako.template import Template
from mako import exceptions
import yaml
import os

def generate(templatefile: str, specfile: str, debug:bool=False):
    template = open(templatefile, 'r').read()
    spec = open(specfile, 'r')
    config = yaml.safe_load(spec)
    if(debug):
        print(config)
        print(yaml.dump(config))

    generated = Template(template).render(**config)
    tpath, ext = os.path.splitext(templatefile)
    tname = tpath.split('/')[-1]
    os.makedirs(config["DataType"] + "Generated", exist_ok=True)
    pathname = config["DataType"] + "Generated/" + config["DataType"] + tname + ext

    # cpp_pathname = config["DataType"] + "Generated/" + config["DataType"] + tname + ".cpp"
    # if not os.path.exists(cpp_pathname):
    #     with open(cpp_pathname, 'wb') as of:
    #         pass

    with open(f"./{pathname}", 'wb') as of:
        of.write(generated.encode())


def generateCMakeLists(templatefile: str, specfile: str,
                       root:bool=False,
                       mode:str="wb", debug:bool=False):
    template = open(templatefile, 'r').read()
    spec = open(specfile, 'r')
    config = yaml.safe_load(spec)
    if(debug):
        print(config)
        print(yaml.dump(config))

    generated = Template(template).render(**config)
    tpath, ext = os.path.splitext(templatefile)
    tname = tpath.split('/')[-1]
    if(root):
        pathname = "./CMakeLists.txt"
    else:
        os.makedirs(config["DataType"] + "Generated", exist_ok=True)
        pathname = config["DataType"] + "Generated/" + "CMakeLists.txt"

    with open(f"./{pathname}", mode) as of:
        of.write(generated.encode())


def generate_all(debug: bool=False):
    itchTemplates = [os.path.join("itch/", file) for file in os.listdir("itch/")]
    itchSpecs = [os.path.join("itch_specs/", file) for file in os.listdir("itch_specs/")]

    if(debug):
        print(itchTemplates)
        print(itchSpecs)

    for bspec in itchSpecs:
        for btem in itchTemplates:
            generate(btem, bspec, debug)

    # generate root CMakeLists
    for i, spec in enumerate(itchSpecs):
        mode = "ab" if i > 0 else "wb"
        generateCMakeLists("./cmake/RootCMakeTemplate.txt", spec,
                           root=True,
                           debug=bool(args.debug), mode=mode)

    # generate subdir CMakeLists
    for spec in itchSpecs:
        generateCMakeLists("./cmake/SubDirCMakeTemplate.txt", spec,
                           root=False, debug=bool(args.debug), mode="wb")



def main(args):
    if(bool(args.all)):
        return generate_all(bool(args.debug))
    return generate(args.templatefile, args.specfile, bool(args.debug))


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    required = parser.add_argument_group("required arguments")
    optional = parser.add_argument_group("optional arguments")
    optional.add_argument("-a", "--all", type=int, help="generate all templates")
    optional.add_argument("-t", "--templatefile",
                        type=str,
                        help="template file path")
    optional.add_argument("-s", "--specfile",
                          type=str,
                          help="spec file path")
    optional.add_argument("-d", "--debug",
                          type=int,
                          help="debug print")
    args = parser.parse_args()

    main(args)
import argument
import function
import class_
import module
import file

import itertools

# The set of Array types we support in Python
arrayTypes = []

for scalar in [("int32_t", "Int32"),
               ("int64_t", "Int64"),
               ("uint32_t", "UInt32"),
               ("uint64_t", "UInt64"),
               ("float", "Float"),
               ("double", "Double"),
               ("lrc::Complex<float>", "ComplexFloat"),
               ("lrc::Complex<double>", "ComplexDouble")
               ]:
    for backend in ["CPU"]:  # ["CPU", "OpenCL", "CUDA"]:
        arrayTypes.append({
            "scalar": scalar[0],
            "backend": backend,
            "name": f"Array{scalar[1]}{backend}"
        })


def generateCppArrayType(config):
    return f"lrc::Array<{config['scalar']}, lrc::backend::{config['backend']}>"


def generateCppArrayViewType(config):
    return f"lrc::array::GeneralArrayView<{generateCppArrayType(config)} &, lrc::Shape>"


def generateFunctionsForArray(config):
    methods = [
        # Default constructor
        function.Function(
            name="__init__",
            args=[]
        ),

        # Copy constructor
        function.Function(
            name="__init__",
            args=[
                argument.Argument(
                    name="other",
                    type=generateCppArrayType(config),
                    const=True,
                    ref=True
                )
            ]
        ),

        # Shape
        function.Function(
            name="__init__",
            args=[
                argument.Argument(
                    name="shape",
                    type="lrc::Shape",
                    const=True,
                    ref=True
                )
            ]
        ),

        # Shape and fill
        function.Function(
            name="__init__",
            args=[
                argument.Argument(
                    name="shape",
                    type="lrc::Shape",
                    const=True,
                    ref=True
                ),
                argument.Argument(
                    name="val",
                    type=config['scalar'],
                    const=True,
                    ref=True
                )
            ]
        ),

        # Copy constructor
        function.Function(
            name="__init__",
            args=[
                argument.Argument(
                    name="other",
                    type=generateCppArrayType(config),
                    const=True,
                    ref=True
                )
            ]
        ),
    ]

    # Static fromData (n dimensions)
    for n in range(1, 9):
        cppType = ("std::vector<" * n) + config['scalar'] + (">" * n)

        methods.append(
            function.Function(
                name="__init__",
                args=[
                    argument.Argument(
                        name=f"array{n}D",
                        type=cppType,
                        const=True,
                        ref=True,
                    )
                ]
            )
        )

    methods += [
        # Get item
        function.Function(
            name="__getitem__",
            args=[
                argument.Argument(
                    name="self",
                    type=generateCppArrayType(config),
                    const=False,
                    ref=True
                ),
                argument.Argument(
                    name="index",
                    type="int64_t"
                )
            ],
            op="""
                return self[index];
            """
        ),

        # Set item (GeneralArrayView)
        function.Function(
            name="__setitem__",
            args=[
                argument.Argument(
                    name="self",
                    type=generateCppArrayType(config),
                    const=False,
                    ref=True
                ),
                argument.Argument(
                    name="index",
                    type="int64_t"
                ),
                argument.Argument(
                    name="other",
                    type=generateCppArrayViewType(config),
                    const=True,
                    ref=True
                )
            ],
            op="""
                self[index] = other;
                return self;
            """
        ),

        # Set item (Array)
        function.Function(
            name="__setitem__",
            args=[
                argument.Argument(
                    name="self",
                    type=generateCppArrayType(config),
                    const=False,
                    ref=True
                ),
                argument.Argument(
                    name="index",
                    type="int64_t"
                ),
                argument.Argument(
                    name="other",
                    type=generateCppArrayType(config),
                    const=True,
                    ref=True
                )
            ],
            op="""
                self[index] = other;
                return self;
            """
        ),

        # Set item (Scalar)
        function.Function(
            name="__setitem__",
            args=[
                argument.Argument(
                    name="self",
                    type=generateCppArrayType(config),
                    const=False,
                    ref=True
                ),
                argument.Argument(
                    name="index",
                    type="int64_t"
                ),
                argument.Argument(
                    name="other",
                    type=config["scalar"],
                    const=True,
                    ref=True
                )
            ],
            op="""
                self[index] = other;
                return self;
            """
        )
    ]

    for operation in [("add", "+"), ("sub", "-"), ("mul", "*"), ("div", "/")]:
        for dtype in [generateCppArrayType(config), generateCppArrayViewType(config), config["scalar"]]:
            methods.append(
                function.Function(
                    name=f"__i{operation[0]}__",
                    args=[
                        argument.Argument(
                            name="self",
                            type=generateCppArrayType(config),
                            const=False,
                            ref=True
                        ),
                        argument.Argument(
                            name="other",
                            type=dtype,
                            const=True,
                            ref=True
                        )
                    ],
                    op=f"""
                        self += other;
                        return self;
                    """
                )
            )

            methods.append(
                function.Function(
                    name=f"__{operation[0]}__",
                    args=[
                        argument.Argument(
                            name="self",
                            type=generateCppArrayType(config),
                            const=True,
                            ref=True
                        ),
                        argument.Argument(
                            name="other",
                            type=dtype,
                            const=True,
                            ref=True
                        )
                    ],
                    op=f"""
                        return (self {operation[1]} other).eval();
                    """
                )
            )

            methods.append(
                function.Function(
                    name=f"__r{operation[0]}__",
                    args=[
                        argument.Argument(
                            name="self",
                            type=generateCppArrayType(config),
                            const=True,
                            ref=True
                        ),
                        argument.Argument(
                            name="other",
                            type=dtype,
                            const=True,
                            ref=True
                        )
                    ],
                    op=f"""
                        return (other {operation[1]} self).eval();
                    """
                )
            )

    methods += [
        # String representation
        function.Function(
            name="__str__",
            args=[
                argument.Argument(
                    name="self",
                    type=generateCppArrayType(config),
                    const=True,
                    ref=True
                )
            ],
            op="""
                return fmt::format("{}", self);
            """
        ),

        # String representation
        function.Function(
            name="__repr__",
            args=[
                argument.Argument(
                    name="self",
                    type=generateCppArrayType(config),
                    const=True,
                    ref=True
                )
            ],
            op=f"""
                std::string thisStr = fmt::format("{{}}", self);
                std::string padded;
                for (const auto &c : thisStr) {{
                    padded += c;
                    if (c == '\\n') {{
                        padded += std::string(16, ' ');
                    }}
                }}
                return fmt::format("<librapid.Array {{}} dtype={config['scalar']} backend={config['backend']}>", padded);
            """
        ),

        # Format (__format__)
        function.Function(
            name="__format__",
            args=[
                argument.Argument(
                    name="self",
                    type=generateCppArrayType(config),
                    const=True,
                    ref=True
                ),
                argument.Argument(
                    name="formatSpec",
                    type="std::string",
                    const=True,
                    ref=True
                )
            ],
            op="""
                std::string format = fmt::format("{{:{}}}", formatSpec);
                return fmt::format(fmt::runtime(format), self);
            """
        )
    ]

    functions = [
        # Transpose
        function.Function(
            name="transpose",
            args=[
                argument.Argument(
                    name="array",
                    type=generateCppArrayType(config),
                    const=True,
                    ref=True
                )
            ],
            op="""
                return lrc::transpose(array).eval();
            """
        ),

        # Matmul
        function.Function(
            name="dot",
            args=[
                argument.Argument(
                    name="a",
                    type=generateCppArrayType(config),
                    const=True,
                    ref=True
                ),
                argument.Argument(
                    name="b",
                    type=generateCppArrayType(config),
                    const=True,
                    ref=True
                )
            ],
            op="""
                return lrc::dot(a, b).eval();
            """
        )
    ]

    for f in [
        "sin",
        "cos",
        "tan",
        "asin",
        "acos",
        "atan",
        "sinh",
        "cosh",
        "tanh",
        "asinh",
        "acosh",
        "atanh",
        "sqrt",
        "cbrt",
        "exp",
        "exp2",
        "exp10",
        "log",
        "log2",
        "log10"
    ]:
        functions.append(
            function.Function(
                name=f,
                args=[
                    argument.Argument(
                        name="array",
                        type=generateCppArrayType(config),
                        const=True,
                        ref=True
                    )
                ],
                op=f"""
                    return lrc::{f}(array).eval();
                """
            )
        )

    if config["backend"] == "CPU":
        functions += [
            function.Function(
                name=f"serialize{config['name']}",
                args=[
                    argument.Argument(
                        name="array",
                        type=generateCppArrayType(config),
                        const=True,
                        ref=True
                    ),
                    argument.Argument(
                        name="path",
                        type="std::string",
                        const=True,
                        ref=True
                    )
                ],
                op="""
                    return lrc::serialize::Serializer(array).write(path);
                """
            ),

            function.Function(
                name=f"deserialize{config['name']}",
                args=[
                    argument.Argument(
                        name="path",
                        type="std::string",
                        const=True,
                        ref=True
                    )
                ],
                op=f"""
                    lrc::serialize::Serializer<{generateCppArrayType(config)}> serializer;
                    serializer.read(path);
                    return serializer.deserialize();
                """
            )
        ]

    return methods, functions


def generateArrayModule(config):
    arrayClass = class_.Class(
        name=config["name"],
        type=generateCppArrayType(config)
    )

    methods, functions = generateFunctionsForArray(config)
    arrayClass.functions.extend(methods)

    includeGuard = None
    if config["backend"] == "CUDA":
        includeGuard = "defined(LIBRAPID_HAS_CUDA)"
    elif config["backend"] == "OpenCL":
        includeGuard = "defined(LIBRAPID_HAS_OPENCL)"

    arrayModule = module.Module(
        name=f"librapid.{config['name']}",
        includeGuard=includeGuard
    )

    arrayModule.addClass(arrayClass)
    arrayModule.functions.extend(functions)

    return arrayModule


def writeArray(root, config):
    fileType = file.File(
        path=f"{root}/{config['name']}.cpp"
    )
    fileType.modules.append(generateArrayModule(config))
    interfaceFunctions = fileType.write()
    return interfaceFunctions


def write(root):
    interfaces = []
    for config in arrayTypes:
        interfaces.extend(writeArray(root, config))
    return interfaces

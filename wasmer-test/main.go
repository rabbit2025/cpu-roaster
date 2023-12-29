package main

import (
	"fmt"
	"io/ioutil"

	wasmer "github.com/wasmerio/wasmer-go/wasmer"
)

func main() {
	wasmBytes, err := ioutil.ReadFile("/data/roaster1.wasm")
	check(err)

	store := wasmer.NewStore(wasmer.NewEngine())
	module, err := wasmer.NewModule(store, wasmBytes)
	check(err)

	wasiEnv, _ := wasmer.NewWasiStateBuilder("wasi-program").
		// Choose according to your actual situation
		// Argument("--foo").
		// Environment("ABC", "DEF").
		// MapDirectory("./", ".").
		Finalize()
	importObject, err := wasiEnv.GenerateImportObject(store, module)
	check(err)

	importObject.Register("env", map[string]wasmer.IntoExtern{
		"emscripten_notify_memory_growth": wasmer.NewFunction(
			store,
			wasmer.NewFunctionType(wasmer.NewValueTypes(wasmer.I32), wasmer.NewValueTypes()),
			func(v []wasmer.Value) ([]wasmer.Value, error) {
				fmt.Println("memory growth!")
				return nil, nil
			},
		),
	})

	instance, err := wasmer.NewInstance(module, importObject)
	check(err)

	// start, err := instance.Exports.GetWasiStartFunction()
	// check(err)
	// start()

	fmt.Println("POW")
	powFn, err := instance.Exports.GetFunction("cpu_roaster_pow")
	check(err)
	result, err := powFn(0, 1, 2, 3, 4, 5, 6, 7, 1000000, 0, 1000)
	check(err)
	fmt.Println("result:", result)
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}

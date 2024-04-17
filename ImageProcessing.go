// go
package main

import (
	"fmt"
	"image"
	"image/color" // Importing color package for RGBA
	"image/jpeg"
	_ "image/jpeg"
	"net/http"
	"os"
	"path"
	"sync"

	"github.com/nfnt/resize"
)

func Resize_Image(width, height int, inputImg image.Image, resizedImgChan chan<- image.Image) {

	resizedImg := resize.Resize(uint(width), uint(height), inputImg, resize.Lanczos3)
	resizedImgChan <- resizedImg
}

func greyScale(pixels *[][]color.Color) {
	ppixels := *pixels
	xLen := len(ppixels)
	yLen := len(ppixels[0])
	// Create new image
	newImage := make([][]color.Color, xLen)
	for i := 0; i < len(newImage); i++ {
		newImage[i] = make([]color.Color, yLen)
	}
	// Process pixels in parallel
	wg := sync.WaitGroup{}
	for x := 0; x < xLen; x++ {
		for y := 0; y < yLen; y++ {
			wg.Add(1)
			go func(x, y int) {
				defer wg.Done()
				pixel := ppixels[x][y]
				originalColor, ok := color.RGBAModel.Convert(pixel).(color.RGBA)
				if !ok {
					fmt.Println("type conversion went wrong")
					return
				}
				grey := uint8(float64(originalColor.R)*0.21 + float64(originalColor.G)*0.72 + float64(originalColor.B)*0.07)
				col := color.RGBA{
					grey,
					grey,
					grey,
					originalColor.A,
				}
				newImage[x][y] = col
			}(x, y)
		}
	}
	wg.Wait()
	*pixels = newImage
}

func flipImageVertical(img image.Image, flippedImgChan chan<- image.Image, wg *sync.WaitGroup, mu *sync.Mutex) {
	defer wg.Done()
	bounds := img.Bounds()
	flippedImg := image.NewRGBA(bounds)

	for x := 0; x < bounds.Dx(); x++ {
		for y := 0; y < bounds.Dy(); y++ {
			// Flip the y-coordinate
			newY := bounds.Dy() - y - 1
			// Copy the pixel from the original image to the flipped image
			flippedImg.Set(x, newY, img.At(x, y))
		}
	}
	// Lock the mutex before sending the flipped image
	mu.Lock()
	flippedImgChan <- flippedImg
	mu.Unlock()
}

func main() {

	// Open the image file
	file, err := os.Open("sample.jpg")
	if err != nil {
		fmt.Println("Error opening image:", err)
		return
	}
	defer file.Close()

	// Decode the image
	inputImg, _, err := image.Decode(file)
	if err != nil {
		fmt.Println("Error decoding image:", err)
		return
	}

	var width, height int

	fmt.Print("Enter width: ")
	_, err1 := fmt.Scan(&width)
	if err1 != nil {
		fmt.Println("Error reading width:", err)
		return
	}

	/***************RESIZING THE IMAGE ***********************/

	// Prompt the user to enter height
	fmt.Print("Enter height: ")
	_, err1 = fmt.Scan(&height)
	if err1 != nil {
		fmt.Println("Error reading height:", err)
		return
	}
	resizedImgChan := make(chan image.Image)

	// Launch a goroutine for resizing
	go Resize_Image(width, height, inputImg, resizedImgChan)

	// Wait for the resized image from the goroutine
	resizedImg := <-resizedImgChan
	// Save the image as output.jpeg
	output, err := os.Create("output.jpeg")
	if err != nil {
		fmt.Println("Error creating output file:", err)
		return
	}
	defer output.Close()

	// Wait for the resized image from the goroutine
	// resizedImg := <-resizedImgChan

	err = jpeg.Encode(output, resizedImg, nil)
	if err != nil {
		fmt.Println("Error encoding and saving image:", err)
		return
	}

	fmt.Println("Image saved as output.jpeg")
	// Display image details
	bounds := inputImg.Bounds()
	fmt.Println("Image format:", bounds.Dx(), "x", bounds.Dy())

	/***************GRAYING IMAGE ***********************/

	// Launch a goroutine for resizing
	go Resize_Image(width, height, inputImg, resizedImgChan)

	// Wait for the resized image from the goroutine
	resizedImg = <-resizedImgChan

	// Create an RGBA image with the same dimensions
	// Apply grayscale
	ppixels := make([][]color.Color, width)
	for i := 0; i < width; i++ {
		ppixels[i] = make([]color.Color, height)
		for j := 0; j < height; j++ {
			ppixels[i][j] = resizedImg.At(i, j)
		}
	}
	greyScale(&ppixels)

	// Create a new image from the grayscale pixels
	newImg := image.NewRGBA(image.Rect(0, 0, width, height))
	for x := 0; x < width; x++ {
		for y := 0; y < height; y++ {
			newImg.Set(x, y, ppixels[x][y])
		}
	}

	// Save the image as output.jpeg
	output, err = os.Create("output1.jpeg")
	if err != nil {
		fmt.Println("Error creating output file:", err)
		return
	}
	defer output.Close()

	// Wait for the resized image from the goroutine
	// resizedImg := <-resizedImgChan

	err = jpeg.Encode(output, newImg, nil)
	if err != nil {
		fmt.Println("Error encoding and saving image:", err)
		return
	}

	fmt.Println("Image saved as output1.jpeg")
	// Display image details
	bounds = inputImg.Bounds()
	fmt.Println("Image format:", bounds.Dx(), "x", bounds.Dy())

	/***************FLIPPING IMAGE UPSIDE DOWN***********************/

	flippedImgChan := make(chan image.Image)
	var wg sync.WaitGroup
	var mu sync.Mutex

	wg.Add(1)
	// Launch a goroutine to flip the image
	go flipImageVertical(inputImg, flippedImgChan, &wg, &mu)

	go func() {
		wg.Wait()
		close(flippedImgChan)
	}()

	for flippedImg := range flippedImgChan {
		output, err := os.Create("output2.jpeg")
		if err != nil {
			fmt.Println("Error creating output file:", err)
			return
		}
		defer output.Close()

		err = jpeg.Encode(output, flippedImg, nil)
		if err != nil {
			fmt.Println("Error encoding and saving image:", err)
			return
		}

		fmt.Println("Image saved as output2.jpeg")

	}

	cwd, err := os.Getwd()
	if err != nil {
		fmt.Println("Error getting current directory:", err)
		return
	}
	// fmt.Println("cmd----", cwd)

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		// HTML code to display images directly
		html := `
		<!DOCTYPE html>
		<html>
		<head>
			<title>Image Processing</title>
		</head>
		<body>
			<h1>Image Processing Results</h1>
			<h2>Input Image</h2>
			<img src="/input" alt="Input Image" width="400">
			<h2>Resized Image</h2>
			<img src="/output" alt="Resized Image" width="400">
			<h2>Grayscale Image</h2>
			<img src="/output1" alt="Grayscale Image" width="400">
			<h2>Flipped Image</h2>
			<img src="/output2" alt="Flipped Image" width="400">
		</body>
		</html>
		`
		fmt.Fprint(w, html)
	})

	http.HandleFunc("/input", func(w http.ResponseWriter, r *http.Request) {
		filePath := path.Join(cwd, "sample.jpg")
		http.ServeFile(w, r, filePath)
	})

	http.HandleFunc("/output", func(w http.ResponseWriter, r *http.Request) {
		filePath := path.Join(cwd, "output.jpeg")
		http.ServeFile(w, r, filePath)
	})

	http.HandleFunc("/output1", func(w http.ResponseWriter, r *http.Request) {
		filePath := path.Join(cwd, "output1.jpeg")
		http.ServeFile(w, r, filePath)
	})

	http.HandleFunc("/output2", func(w http.ResponseWriter, r *http.Request) {
		filePath := path.Join(cwd, "output2.jpeg")
		http.ServeFile(w, r, filePath)
	})

	var wg1 sync.WaitGroup
	wg1.Add(1)

	go func() {
		defer wg1.Done()
		fmt.Println("Server running on http://localhost:8000")
		err := http.ListenAndServe(":8000", nil)
		if err != nil {
			fmt.Println("Error starting server:", err)
		}
	}()

	wg1.Wait() // Block the main goroutine

}

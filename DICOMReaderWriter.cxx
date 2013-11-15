
#include "DICOMReaderWriterCLP.h"

// ITK includes
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkDICOMImageIO2.h"
#include "itkDICOMSeriesFileNames.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkGDCMImageIO.h"
#include "itkDCMTKImageIO.h"
#include "itkDCMTKSeriesFileNames.h"
#include "itkMetaDataObject.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkNumericSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include <vector>
#include "itksys/SystemTools.hxx"
#include "itkPluginUtilities.h"

namespace
{

}

int main(int argc, char* argv[])
{
  PARSE_ARGS;

  typedef itk::Image<short,3>                  Image3DType;
  typedef itk::ImageSeriesReader< Image3DType >  ReaderType;

  typedef itk::GDCMImageIO            gdcmIOType;
  typedef itk::DCMTKImageIO           dcmtkIOType;        
  typedef itk::GDCMSeriesFileNames    gdcmNamesGeneratorType;
  typedef itk::DCMTKSeriesFileNames   dcmtkNamesGeneratorType;

  typedef itk::ImageFileWriter< Image3DType > WriterType;
  
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  typedef signed short OutputPixelType;
  const unsigned int OutputDimension = 2;

  typedef itk::Image< OutputPixelType, OutputDimension> image2DType;

  //GDCM Reader-Writer
  if( ioType == "GDCM" )
  {
    gdcmIOType::Pointer gdcmIO = gdcmIOType::New();
    gdcmNamesGeneratorType::Pointer gdcmNamesGenerator = gdcmNamesGeneratorType::New();
    gdcmNamesGenerator->SetInputDirectory( inputDirectory );
  
    const ReaderType::FileNamesContainer & filenames =
                            gdcmNamesGenerator->GetInputFileNames();

    reader->SetImageIO( gdcmIO );
    reader->SetFileNames( filenames );
    try
    {
      reader->Update();
    }
    catch (itk::ExceptionObject &excp)
    {
      std::cerr << "Exception thrown while writing the image" << std::endl;
      std::cerr << excp << std::endl;
      return EXIT_FAILURE;
    }

    typedef itk::ImageSeriesWriter<
            Image3DType,image2DType> SeriesWriterType;
    SeriesWriterType::Pointer seriesWriter = SeriesWriterType::New();
    seriesWriter->SetInput(reader->GetOutput());
    seriesWriter->SetImageIO(gdcmIO);
    gdcmNamesGenerator->SetOutputDirectory( outputDirectory);
    seriesWriter->SetFileNames(gdcmNamesGenerator->GetOutputFileNames());
    seriesWriter->SetMetaDataDictionaryArray(
                            reader->GetMetaDataDictionaryArray() );
    try
       {
       seriesWriter->Update();
       }
     catch( itk::ExceptionObject & excp )
       {
       std::cerr << "Exception thrown while writing the series " << std::endl;
       std::cerr << excp << std::endl;
       return EXIT_FAILURE;
       }


  }

  //DCMTK Reader-Writer
  else //DCMTK
  {
    dcmtkIOType::Pointer dcmtkIO = dcmtkIOType::New();
    dcmtkNamesGeneratorType::Pointer dcmtkNamesGenerator = dcmtkNamesGeneratorType::New();
    dcmtkNamesGenerator->SetInputDirectory( inputDirectory );
  
    const ReaderType::FileNamesContainer & filenames =
                            dcmtkNamesGenerator->GetInputFileNames();

    reader->SetImageIO( dcmtkIO );
    reader->SetFileNames( filenames );

    try
    {
      reader->Update();
    }
    catch (itk::ExceptionObject &excp)
    {
      std::cerr << "Exception thrown while writing the image" << std::endl;
      std::cerr << excp << std::endl;
      return EXIT_FAILURE;
    }
  }

  writer->SetFileName( outputVolume.c_str() );
  writer->SetInput( reader->GetOutput() );
  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject &ex)
  {
    std::cout << ex;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

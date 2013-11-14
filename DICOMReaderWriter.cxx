
#include "DICOMReaderWriterCLP.h"

// ITK includes
#include "itkImageSeriesReader.h"
#include "itkDICOMImageIO2.h"
#include "itkDICOMSeriesFileNames.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkGDCMImageIO.h"
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

  typedef itk::Image<short,3>                  ImageType;
  typedef itk::ImageSeriesReader< ImageType >  ReaderType;

  typedef itk::GDCMImageIO                        ImageIOType;
  typedef itk::GDCMSeriesFileNames                NamesGeneratorType;

  typedef itk::Image<ImageType, 3>         Image3DType;
  typedef itk::Image<ImageType, 2>         Image2DType;

  //typedef itk::ImageFileWriter<Image2DType>     WriterType;

  typedef itk::ImageFileWriter< ImageType > WriterType;

  WriterType::Pointer writer = WriterType::New();

  ImageIOType::Pointer gdcmIO = ImageIOType::New();

  // Reading the input DICOCM directory and get the
  // attribute values which needs to be copied to the
  //output DICOM directory

  NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();
  namesGenerator->SetInputDirectory( inputDirectory );

  const ReaderType::FileNamesContainer & filenames =
                            namesGenerator->GetInputFileNames();

  ReaderType::Pointer reader = ReaderType::New();

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

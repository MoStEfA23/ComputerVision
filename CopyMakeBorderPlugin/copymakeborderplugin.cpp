#include "copymakeborderplugin.h"
#include "ui_plugin.h"

CopyMakeBorderPlugin::CopyMakeBorderPlugin():
    QObject()
{
}

CopyMakeBorderPlugin::~CopyMakeBorderPlugin()
{

}

QString CopyMakeBorderPlugin::title()
{
    return this->metaObject()->className();
}

QString CopyMakeBorderPlugin::version()
{
    return "1.0.0"; //hard coded version for the moment
}

QString CopyMakeBorderPlugin::description()
{
    return "CopyMakeBorderPlugin";
}

QString CopyMakeBorderPlugin::help()
{
    return "Help";
}

void CopyMakeBorderPlugin::setupUi(QWidget *parent)
{
    ui = new Ui::PluginGui;
    ui->setupUi(parent);

    QStringList items;
    items.append("BORDER_CONSTANT");
    items.append("BORDER_REPLICATE");
    items.append("BORDER_REFLECT");
    items.append("BORDER_WRAP");
    items.append("BORDER_REFLECT_101");

    ui->mBorderTypeComboBox->addItems(items);
    connect(ui->mBorderTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentBorderTypeIndexChanged(int)));
}

void CopyMakeBorderPlugin::processImage(const cv::Mat &inputImage, cv::Mat &outputImage)
{
    inputImage.copyTo(outputImage);

    int top, bot, left, right;
     top = bot = inputImage.rows / 8;
     left = right = inputImage.cols / 8;

     cv::copyMakeBorder(inputImage, outputImage, top, bot, left, right, ui->mBorderTypeComboBox->currentIndex());
}

void CopyMakeBorderPlugin::onCurrentBorderTypeIndexChanged(int index)
{
    Q_UNUSED(index)
    emit updateNeed();
}

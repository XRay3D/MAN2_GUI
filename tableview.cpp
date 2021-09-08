#include "tableview.h"
#include "header.h"
#include "testmodel.h"

#include <QCheckBox>

TableView::TableView(QWidget* parent)
    : QTableView(parent)
{
}

TableView::~TableView() { }

TestModel* TableView::model() const { return m_model; }

void TableView::init()
{
    //    setModel(m_model = new Model(this));
    //    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TableView::initCheckBox()
{
    auto vh = new Header(Qt::Vertical, this);
    setVerticalHeader(vh);
    createCornerCheckBox(vh);

    setModel(m_model = new TestModel(this, nullptr, vh->checked()));
    connect(vh, &Header::onChecked, m_model, &TestModel::onChecked);

    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TableView::initRadioButton()
{
    auto hh = new Header(Qt::Horizontal, this);
    setHorizontalHeader(hh);

    //    setModel(m_model = new Model(this, hh->checked()));
    //    connect(hh, &Header::onChecked, m_model, &Model::onChecked);

    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TableView::initCheckBoxRadioButton()
{
    auto hh = new Header(Qt::Horizontal, this);
    auto vh = new Header(Qt::Vertical, this);

    setHorizontalHeader(hh);
    setVerticalHeader(vh);
    createCornerCheckBox(vh);

    //    setModel(m_model = new Model(this, hh->checked(), vh->checked()));
    //    connect(hh, &Header::onChecked, m_model, &Model::onChecked);
    //    connect(vh, &Header::onChecked, m_model, &Model::onChecked);

    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TableView::createCornerCheckBox(Header* header)
{
    auto cornerButton = findChild<QAbstractButton*>();
    if (cornerButton) {
        m_checkBox = new QCheckBox(cornerButton);
        m_checkBox->setFocusPolicy(Qt::NoFocus);
        m_checkBox->setGeometry(Header::getRect(cornerButton->rect()).translated(1, -4));
        connect(m_checkBox, &QCheckBox::clicked, [header](bool checked) { header->setAll(checked); });
        connect(header, &Header::onCheckedV, [this](const QVector<bool>& v, int) {
            static const Qt::CheckState chState[] {
                Qt::Unchecked,
                Qt::Unchecked,
                Qt::Checked,
                Qt::PartiallyChecked
            };
            m_checkBox->setCheckState(chState[v.contains(true) * 2 | v.contains(false) * 1]);
        });
    }
}

QCheckBox* TableView::checkBox() const
{
    return m_checkBox;
}

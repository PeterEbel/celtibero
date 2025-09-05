from reportlab.lib.pagesizes import A4
from reportlab.pdfgen import canvas

def create_test_pdf(filename):
    c = canvas.Canvas(filename, pagesize=A4)
    width, height = A4

    # Seite 1
    c.drawString(100, height - 100, "Test-PDF: ARTICABE, AUCI und GERGIUM.")
    c.showPage()
    # Seite 2
    c.drawString(100, height - 100, "Test-PDF: PRECORIUM, LATURICUS und ABISONIUS.")
    c.showPage()
    # Seite 3
    c.drawString(100, height - 100, "Test-PDF: PIRICANTIN, PIRICANTIO und AKOSON.")
    c.showPage()
    # Seite 4
    c.drawString(100, height - 100, "Test-PDF: ARS, ARSA, ARSAKOS und ARZA.")
    c.showPage()

    c.save()

if __name__ == "__main__":
    create_test_pdf("testbegriffe.pdf")

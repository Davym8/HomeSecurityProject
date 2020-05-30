import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.image import MIMEImage
"""
This program is used and imported to the main program for execution
sends an email with an image

"""
# Email you want to send the update from (only works with gmail)
fromEmail = ('david.j.adams117@gmail.com')
# You can generate an app password here to avoid storing your password in plain text
# https://support.google.com/accounts/answer/185833?hl=en
#DO NOT USE PLAIN TEXT
fromEmailPassword = ('ifxofzjrcfvfubqb')

# Email you want to send the update to
toEmail = ('david.j.adams117@gmail.com')

def sendEmail(image):
    msgRoot = MIMEMultipart('related')
    msgRoot['Subject'] = ('Security Update')
    msgRoot['From'] = fromEmail
    msgRoot['To'] = toEmail
    msgRoot.preamble = ('Raspberry pi security camera update')

    msgAlternative = MIMEMultipart('alternative')
    msgRoot.attach(msgAlternative)
    msgText = MIMEText('Smart security cam found object')
    msgAlternative.attach(msgText)

    msgText = MIMEText('<img src="cid:image1">', 'html')
    msgAlternative.attach(msgText)

    msgImage = MIMEImage(image)
    msgImage.add_header('Content-ID', '<image1>')
    msgRoot.attach(msgImage)

    smtp = smtplib.SMTP('smtp.gmail.com', 587)
    smtp.starttls()
    smtp.login(fromEmail, fromEmailPassword)
    smtp.sendmail(fromEmail, toEmail, msgRoot.as_string())
    smtp.quit()
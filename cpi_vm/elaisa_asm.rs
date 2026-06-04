#![expect(unused)]

#[repr(u8)]
enum LineKind {
    C,
    Var,
    LoadTxt,
    StoreTxt,
    Exit,
    Label,
    CmpZs,
    CmpZ,
    Cmp,
    RetEq,
    RetNeq,
    Ret,
    Jmp,
    Inc,
    EmptyLine,
}

type Word<'a> = (u8, &'a str);
type Words<'a> = Vec<Word<'a>>;

fn lex<'a>(lines: &'a str) -> Words {
    fn lex_line<'a>(line: &'a str) -> Words {
        let mut out = Vec::<Word>::new();

        let words = line.split_whitespace().collect::<Vec<&str>>();
        if words.is_empty() {
            return out;
        }

        for word in &words {
            let kind: u8 = match words[0] {
                "c" => LineKind::C,
                "var" => LineKind::Var,
                "load_txt" => LineKind::LoadTxt,
                "store_txt" => LineKind::StoreTxt,
                "exit" => LineKind::Exit,
                "label" => LineKind::Label,
                "cmp.zs" => LineKind::CmpZs,
                "cmp.z" => LineKind::CmpZ,
                "cmp" => LineKind::Cmp,
                "ret.eq" => LineKind::RetEq,
                "ret.neq" => LineKind::RetNeq,
                "ret" => LineKind::Ret,
                "jmp" => LineKind::Jmp,
                "inc" => LineKind::Inc,
                _ => LineKind::EmptyLine,
            } as u8;
            Vec::<Word>::push(&mut out, (kind, word));
        }

        out
    }

    let mut out = Vec::<Word>::new();
    for line in lines.lines() {
        let mut l = lex_line(line);
        Vec::<Word>::append(&mut out, &mut l);
    }

    out
}

fn assemble(lines: &str) -> Vec<u8> {
    let toks = lex(lines);
    for tok in toks {
        match tok {

        }
    }
}

fn main() {}

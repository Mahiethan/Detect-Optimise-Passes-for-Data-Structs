; ModuleID = 'globalPointerSoA.c'
source_filename = "globalPointerSoA.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.StructureOne = type { [100000 x i32], [200000 x i32], [300000 x i8] }

@.str = private unnamed_addr constant [11 x i8] c"Array a: \0A\00", align 1
@.str.1 = private unnamed_addr constant [14 x i8] c"Index %d: %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"Array b: \0A\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"Array c: \0A\00", align 1
@.str.4 = private unnamed_addr constant [14 x i8] c"Index %d: %c\0A\00", align 1
@gs1 = dso_local global ptr null, align 8

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateStructure(ptr noundef %soa, i32 noundef %sizeA, i32 noundef %sizeB, i32 noundef %sizeC) #0 {
entry:
  %soa.addr = alloca ptr, align 8
  %sizeA.addr = alloca i32, align 4
  %sizeB.addr = alloca i32, align 4
  %sizeC.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %soa, ptr %soa.addr, align 8
  store i32 %sizeA, ptr %sizeA.addr, align 4
  store i32 %sizeB, ptr %sizeB.addr, align 4
  store i32 %sizeC, ptr %sizeC.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %sizeA.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32, ptr %i, align 4
  %3 = load ptr, ptr %soa.addr, align 8
  %a = getelementptr inbounds %struct.StructureOne, ptr %3, i32 0, i32 0
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds [100000 x i32], ptr %a, i64 0, i64 %idxprom
  store i32 %2, ptr %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %5 = load i32, ptr %i, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc6, %for.end
  %6 = load i32, ptr %i, align 4
  %7 = load i32, ptr %sizeB.addr, align 4
  %cmp2 = icmp slt i32 %6, %7
  br i1 %cmp2, label %for.body3, label %for.end8

for.body3:                                        ; preds = %for.cond1
  %8 = load i32, ptr %i, align 4
  %9 = load ptr, ptr %soa.addr, align 8
  %b = getelementptr inbounds %struct.StructureOne, ptr %9, i32 0, i32 1
  %10 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %10 to i64
  %arrayidx5 = getelementptr inbounds [200000 x i32], ptr %b, i64 0, i64 %idxprom4
  store i32 %8, ptr %arrayidx5, align 4
  br label %for.inc6

for.inc6:                                         ; preds = %for.body3
  %11 = load i32, ptr %i, align 4
  %inc7 = add nsw i32 %11, 1
  store i32 %inc7, ptr %i, align 4
  br label %for.cond1, !llvm.loop !8

for.end8:                                         ; preds = %for.cond1
  store i32 0, ptr %i, align 4
  br label %for.cond9

for.cond9:                                        ; preds = %for.inc14, %for.end8
  %12 = load i32, ptr %i, align 4
  %13 = load i32, ptr %sizeC.addr, align 4
  %cmp10 = icmp slt i32 %12, %13
  br i1 %cmp10, label %for.body11, label %for.end16

for.body11:                                       ; preds = %for.cond9
  %14 = load i32, ptr %i, align 4
  %conv = trunc i32 %14 to i8
  %15 = load ptr, ptr %soa.addr, align 8
  %c = getelementptr inbounds %struct.StructureOne, ptr %15, i32 0, i32 2
  %16 = load i32, ptr %i, align 4
  %idxprom12 = sext i32 %16 to i64
  %arrayidx13 = getelementptr inbounds [300000 x i8], ptr %c, i64 0, i64 %idxprom12
  store i8 %conv, ptr %arrayidx13, align 1
  br label %for.inc14

for.inc14:                                        ; preds = %for.body11
  %17 = load i32, ptr %i, align 4
  %inc15 = add nsw i32 %17, 1
  store i32 %inc15, ptr %i, align 4
  br label %for.cond9, !llvm.loop !9

for.end16:                                        ; preds = %for.cond9
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @freeStructure(ptr noundef %soa) #0 {
entry:
  %soa.addr = alloca ptr, align 8
  store ptr %soa, ptr %soa.addr, align 8
  %0 = load ptr, ptr %soa.addr, align 8
  call void @free(ptr noundef %0) #4
  ret void
}

; Function Attrs: nounwind
declare void @free(ptr noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @printStructure(ptr noundef %soa, i32 noundef %sizeA, i32 noundef %sizeB, i32 noundef %sizeC) #0 {
entry:
  %soa.addr = alloca ptr, align 8
  %sizeA.addr = alloca i32, align 4
  %sizeB.addr = alloca i32, align 4
  %sizeC.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %soa, ptr %soa.addr, align 8
  store i32 %sizeA, ptr %sizeA.addr, align 4
  store i32 %sizeB, ptr %sizeB.addr, align 4
  store i32 %sizeC, ptr %sizeC.addr, align 4
  %call = call i32 (ptr, ...) @printf(ptr noundef @.str)
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %sizeA.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32, ptr %i, align 4
  %3 = load ptr, ptr %soa.addr, align 8
  %a = getelementptr inbounds %struct.StructureOne, ptr %3, i32 0, i32 0
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds [100000 x i32], ptr %a, i64 0, i64 %idxprom
  %5 = load i32, ptr %arrayidx, align 4
  %call1 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %2, i32 noundef %5)
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %6 = load i32, ptr %i, align 4
  %inc = add nsw i32 %6, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !10

for.end:                                          ; preds = %for.cond
  %call2 = call i32 (ptr, ...) @printf(ptr noundef @.str.2)
  store i32 0, ptr %i, align 4
  br label %for.cond3

for.cond3:                                        ; preds = %for.inc9, %for.end
  %7 = load i32, ptr %i, align 4
  %8 = load i32, ptr %sizeB.addr, align 4
  %cmp4 = icmp slt i32 %7, %8
  br i1 %cmp4, label %for.body5, label %for.end11

for.body5:                                        ; preds = %for.cond3
  %9 = load i32, ptr %i, align 4
  %10 = load ptr, ptr %soa.addr, align 8
  %b = getelementptr inbounds %struct.StructureOne, ptr %10, i32 0, i32 1
  %11 = load i32, ptr %i, align 4
  %idxprom6 = sext i32 %11 to i64
  %arrayidx7 = getelementptr inbounds [200000 x i32], ptr %b, i64 0, i64 %idxprom6
  %12 = load i32, ptr %arrayidx7, align 4
  %call8 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %9, i32 noundef %12)
  br label %for.inc9

for.inc9:                                         ; preds = %for.body5
  %13 = load i32, ptr %i, align 4
  %inc10 = add nsw i32 %13, 1
  store i32 %inc10, ptr %i, align 4
  br label %for.cond3, !llvm.loop !11

for.end11:                                        ; preds = %for.cond3
  %call12 = call i32 (ptr, ...) @printf(ptr noundef @.str.3)
  store i32 0, ptr %i, align 4
  br label %for.cond13

for.cond13:                                       ; preds = %for.inc19, %for.end11
  %14 = load i32, ptr %i, align 4
  %15 = load i32, ptr %sizeC.addr, align 4
  %cmp14 = icmp slt i32 %14, %15
  br i1 %cmp14, label %for.body15, label %for.end21

for.body15:                                       ; preds = %for.cond13
  %16 = load i32, ptr %i, align 4
  %17 = load ptr, ptr %soa.addr, align 8
  %c = getelementptr inbounds %struct.StructureOne, ptr %17, i32 0, i32 2
  %18 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %18 to i64
  %arrayidx17 = getelementptr inbounds [300000 x i8], ptr %c, i64 0, i64 %idxprom16
  %19 = load i8, ptr %arrayidx17, align 1
  %conv = sext i8 %19 to i32
  %call18 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i32 noundef %16, i32 noundef %conv)
  br label %for.inc19

for.inc19:                                        ; preds = %for.body15
  %20 = load i32, ptr %i, align 4
  %inc20 = add nsw i32 %20, 1
  store i32 %inc20, ptr %i, align 4
  br label %for.cond13, !llvm.loop !12

for.end21:                                        ; preds = %for.cond13
  ret void
}

declare i32 @printf(ptr noundef, ...) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  %call = call noalias ptr @malloc(i64 noundef 1500000) #5
  store ptr %call, ptr @gs1, align 8
  %0 = load ptr, ptr @gs1, align 8
  call void @populateStructure(ptr noundef %0, i32 noundef 100000, i32 noundef 200000, i32 noundef 300000)
  %1 = load ptr, ptr @gs1, align 8
  call void @freeStructure(ptr noundef %1)
  ret i32 0
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #3

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind }
attributes #5 = { nounwind allocsize(0) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 893416051d517d979481510fe3b52d83ba0d1e01)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
